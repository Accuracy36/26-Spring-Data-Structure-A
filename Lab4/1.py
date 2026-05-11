import sys
import os
import heapq
import struct

# 哈夫曼树节点
class HuffmanNode:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

class MyZip:
    def __init__(self):
        self.codes = {}
        
    # 构建频率表
    def build_frequency_dict(self, text_bytes):
        frequency = {}
        for byte in text_bytes:
            frequency[byte] = frequency.get(byte, 0) + 1
        return frequency

    # 构建哈夫曼树
    def build_huffman_tree(self, frequency):
        priority_queue = [HuffmanNode(char, freq) for char, freq in frequency.items()]
        heapq.heapify(priority_queue)

        while len(priority_queue) > 1:
            left = heapq.heappop(priority_queue)
            right = heapq.heappop(priority_queue)
            merged = HuffmanNode(None, left.freq + right.freq)
            merged.left = left
            merged.right = right
            heapq.heappush(priority_queue, merged)

        return heapq.heappop(priority_queue) if priority_queue else None

    # 生成哈夫曼编码表
    def build_codes_helper(self, root, current_code):
        if root is None:
            return
        if root.char is not None:
            self.codes[root.char] = current_code
            return
        self.build_codes_helper(root.left, current_code + "0")
        self.build_codes_helper(root.right, current_code + "1")

    # 压缩文件
    def compress(self, input_path, output_path):
        with open(input_path, 'rb') as file:
            text_bytes = file.read()

        if not text_bytes:
            print("文件为空，无需压缩。")
            return

        # 1. 统计频率并建树
        frequency = self.build_frequency_dict(text_bytes)
        root = self.build_huffman_tree(frequency)
        self.codes = {}
        self.build_codes_helper(root, "")
        
        # 单字符文件的特殊处理
        if len(frequency) == 1:
            self.codes[root.char] = "0"

        # 2. 将文本转换为 01 字符串
        encoded_bits = "".join([self.codes[byte] for byte in text_bytes])

        # 3. 计算为了凑齐字节需要补零的数量 (Padding)
        extra_padding = (8 - len(encoded_bits) % 8) % 8
        encoded_bits += "0" * extra_padding

        # 4. 写入自定义的压缩文件
        with open(output_path, 'wb') as output:
            # Header 1: 写入独立字符的种类数 (4字节整数)
            output.write(struct.pack('>I', len(frequency)))
            
            # Header 2: 写入完整的频率表 [字符: 1字节] + [频率: 4字节]
            for char, freq in frequency.items():
                output.write(struct.pack('>BI', char, freq))
            
            # Header 3: 写入补零的数量 (1字节)
            output.write(struct.pack('>B', extra_padding))
            
            # Body: 写入压缩后的二进制数据 (按字节写入)
            b = bytearray()
            for i in range(0, len(encoded_bits), 8):
                byte_str = encoded_bits[i:i+8]
                b.append(int(byte_str, 2))
            output.write(bytes(b))
        
        print(f"压缩成功: {input_path} -> {output_path}")

    # 解压文件
    def decompress(self, input_path):
        output_path = "unzipped_" + os.path.basename(input_path).replace('.huf', '.txt')
        
        with open(input_path, 'rb') as file:
            # Header 1: 读取字符种类数
            freq_size_data = file.read(4)
            if not freq_size_data:
                return
            freq_size = struct.unpack('>I', freq_size_data)[0]
            
            # Header 2: 读取频率表并重建哈夫曼树
            frequency = {}
            for _ in range(freq_size):
                char, freq = struct.unpack('>BI', file.read(5))
                frequency[char] = freq
            
            root = self.build_huffman_tree(frequency)
            
            # Header 3: 读取补零的数量
            extra_padding = struct.unpack('>B', file.read(1))[0]
            
            # Body: 读取并展开所有二进制位
            bit_string = ""
            byte = file.read(1)
            while byte:
                byte_val = ord(byte)
                bit_string += bin(byte_val)[2:].rjust(8, '0')
                byte = file.read(1)

        # 截掉尾部为了凑字节而补的 0
        if extra_padding > 0:
            bit_string = bit_string[:-extra_padding]

        # 逐位解码
        decoded_bytes = bytearray()
        current_node = root
        
        # 单字符文件的特殊处理
        if root.left is None and root.right is None:
            decoded_bytes.extend([root.char] * root.freq)
        else:
            for bit in bit_string:
                if bit == '0':
                    current_node = current_node.left
                else:
                    current_node = current_node.right
                    
                if current_node.left is None and current_node.right is None:
                    decoded_bytes.append(current_node.char)
                    current_node = root

        with open(output_path, 'wb') as output:
            output.write(bytes(decoded_bytes))
            
        print(f"解压成功: {input_path} -> {output_path}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("用法说明:")
        print("  压缩: python myzip.py -c input.txt output.huf")
        print("  解压: python myzip.py -d output.huf")
        sys.exit(1)

    command = sys.argv[1]
    myzip = MyZip()

    if command == "-c" and len(sys.argv) == 4:
        myzip.compress(sys.argv[2], sys.argv[3])
    elif command == "-d" and len(sys.argv) == 3:
        myzip.decompress(sys.argv[2])
    else:
        print("参数输入有误。")
