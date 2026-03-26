	#include <iostream>
	#include <string>
	#include <algorithm>


	struct DigitNode {
	    int digit;          // save 0-9
	    DigitNode* next;
	    DigitNode(int d) : digit(d), next(nullptr) {}
	};

	class BigInt {
	public:
	    DigitNode* head;    //the rightmost digit (least significant digit)
	    bool isNegative;

	    BigInt(){
            head = nullptr;
            isNegative = false;
        }


		BigInt(int num){
			head = nullptr;
			isNegative = (num < 0);
			//TODO: begin here
			
			DigitNode* tail = nullptr; // 1. 声明 tail
			
			if(num == 0) {
				appendNode(head, tail, 0);
			}
			else {
				// 3. 用 long long 接住绝对值，防止 INT_MIN 溢出
				long long temp = std::abs((long long)num); 
				
				while(temp > 0) {
					appendNode(head, tail, temp % 10);
					temp /= 10;
				}
			}
			//TODO: end here
		}

	    // construct from string
	    BigInt(const std::string& numStr) {
	        head = nullptr;
	        isNegative = false;
	        if (numStr.empty()) return;
	        int start = 0;
	        if (numStr[0] == '-') {
	            isNegative = true;
	            start = 1;
	        } else if (numStr[0] == '+') {
	            start = 1;
	        }
	        DigitNode* tail = nullptr;
	        // string "123" -> linked list 3 -> 2 -> 1
	        // from 3 to 1
	        for (int i = numStr.length() - 1; i >= start; --i) {
	            if (isdigit(numStr[i])) {
	                appendNode(head, tail, numStr[i] - '0');
	            }
	        }
	    }

	    bool isZero(){      //to check if the number is zero
	        return (head == nullptr) || (head->digit == 0 && head->next == nullptr);
	    }

	    // clear the linked list
	    void clear() {
	        DigitNode* current = head;
	        while (current != nullptr) {
	            DigitNode* next = current->next;
	            delete current;
	            current = next;
	        }
	        head = nullptr;
	    }

	    // add a node to the tail(leftmost digit)
	    static void appendNode(DigitNode*& head, DigitNode*& tail, int val) {
	        DigitNode* newNode = new DigitNode(val);
	        if (!head) {
	            head = tail = newNode;
	        } 
            
            else {
	            tail->next = newNode;
	            tail = newNode;
	        }
	    }



	    // 1 for this > other, -1 for this < other, 0 for equal
	    int compareAbsolute(BigInt& other){
            int len1 = 0, len2 = 0;
            for(DigitNode*p1 = head; p1 != nullptr; p1 = p1->next) len1++;
            for(DigitNode*p2 = other.head; p2 != nullptr; p2 = p2->next) len2++;
            if(len1 > len2) 
                return 1;
            else if(len1 < len2) 
                return -1;
            //TODO: begin here
			int cmp1[len1], cmp2[len2];
			DigitNode*p1 = head;
			DigitNode*p2 = other.head;
			for(int i = 0; i < len1; i++) {
				cmp1[i] = p1->digit;
				p1 = p1->next;
			}
			for(int i = 0; i < len2; i++) {
				cmp2[i] = p2->digit;
				p2 = p2->next;
			}
			for(int i = len1-1; i >=0; i--) {
				if(cmp1[i] > cmp2[i]) return 1;
				else if(cmp1[i] < cmp2[i]) return -1;
			}
			return 0;
            //TODO: end here
	    }

        int compare(BigInt& other){
            //TODO: begin here
			if(isNegative != other.isNegative) {
				if(isNegative) return -1;
				else return 1;
			}
			else {
				if(isNegative) {
					return -compareAbsolute(other);
				}
				else {
					return compareAbsolute(other);
				}
			}
            //TODO: end here
        }





	    BigInt addAbsolute(BigInt& a, BigInt& b) {
	        BigInt result;
	        result.isNegative = false;
            //TODO: begin here
	       int carry=0;
	       DigitNode*p1 = a.head;
	       DigitNode*p2 = b.head;
		   DigitNode* tail = nullptr;
	       while(p1 != nullptr || p2 != nullptr) {
	       	int sum = carry;
	       	if(p1 != nullptr) {
	       		sum += p1->digit;
	       		p1 = p1->next;
	       	}

	       	if(p2 != nullptr) {
	       		sum += p2->digit;
	       		p2 = p2->next;
	       	}
	       	BigInt::appendNode(result.head, tail, sum % 10);
	       	carry = sum / 10;
	       }
	       if(carry > 0) {
	       	BigInt::appendNode(result.head, tail, carry);
	       }
            //TODO: end here
            return result;
	    }

	    BigInt subAbsolute(BigInt& a, BigInt& b) {
	        // suppose a >= b
	        BigInt result;
	        result.isNegative = false;
            //TODO: begin here
	        int borrow=0;
	        DigitNode*p1 = a.head;
	        DigitNode*p2 = b.head;
	        DigitNode* tail = nullptr;
	        while(p1 != nullptr || p2 != nullptr) {
	        	int diff = borrow;
	        	if(p1 != nullptr) {
	        		diff += p1->digit;
	        		p1 = p1->next;
	        	}
				if(p2 != nullptr) {
					diff -= p2->digit;
					p2 = p2->next;
				}
	        	if(diff < 0) {
	        		diff += 10;
	        		borrow = -1;
				}
				else {
					borrow = 0;
				}
				BigInt::appendNode(result.head, tail, diff);
			}
            //TODO: end here
	        result.removeLeadingZeros();
	        return result;
	    }

	    ~BigInt() {
	        clear();
	    }

	    // add
	    BigInt operator+(BigInt& other){
	        BigInt result;
            if (isNegative == other.isNegative) {
                result = addAbsolute(*this, other);
                result.isNegative = isNegative;
            } 
            //TODO: begin here
            else {
				int cmp=compareAbsolute(other);
				if(cmp == 1) {
					result = subAbsolute(*this, other);
					result.isNegative = isNegative;
				}
				else if (cmp==-1){
					result = subAbsolute(other, *this);
					result.isNegative = other.isNegative;
				}
				else{
					result.isNegative = false;
					result = subAbsolute(*this, other);
				}
            }
            //TODO: end here
	        return result;
	    }

	    // sub
	    BigInt operator-(BigInt& other){
            BigInt result;
            //TODO: begin here
			BigInt cpyother=other;
			cpyother.isNegative = !other.isNegative;
			result =operator+( cpyother);
            //TODO: end here
            return result;
	    }

	    // mul
	    BigInt operator*(BigInt& other){
	        BigInt result;

            result.isNegative = (isNegative != other.isNegative);
            DigitNode* p1 = this->head;
            int pos1 = 0;
			DigitNode* tail = nullptr;
            while (p1) {
                BigInt tempRes;
                //TODO: begin here
				for(int i=0;i<pos1;i++) {
					BigInt::appendNode(tempRes.head, tail, 0);
				}
				DigitNode*p2 = other.head;
				int carry=0;
				while(p2 != nullptr) {
					int product = carry;
					product += p1->digit * p2->digit;
					carry = product / 10;
					BigInt::appendNode(tempRes.head, tail, product % 10);
					p2 = p2->next;
				}
				if(carry > 0) {
					BigInt::appendNode(tempRes.head,tail, carry);
				}
				tempRes.isNegative = result.isNegative;
                //TODO: end here
                result = result + tempRes;
                p1 = p1->next;
                pos1++;
            }
 
            if (result.isZero()) result.isNegative = false;
			result.removeLeadingZeros();
            return result;
        }

        std::string toString() const {
            std::string s;
            if (isNegative) s += '-';
            DigitNode* curr = head;
            while (curr) {
                s += (curr->digit + '0');
                curr = curr->next;
            }
            if (s.empty() || s == "-") return "0";
            std::reverse(s.begin() + (isNegative ? 1 : 0), s.end());
            size_t start_pos = s.find_first_not_of('0', isNegative ? 1 : 0);
            if (std::string::npos == start_pos) return "0";
            return s.substr(0, isNegative ? 1 : 0) + s.substr(start_pos);
        }

    BigInt& operator=(const BigInt& other) {
        if (this != &other) {
            clear();
            isNegative = other.isNegative;
            DigitNode* tail = nullptr;
            DigitNode* curr = other.head;
            while (curr) {
                appendNode(head, tail, curr->digit);
                curr = curr->next;
            }
        }
        return *this;
    }

	BigInt(const BigInt& other) {
	    head = nullptr;     
	    isNegative = other.isNegative;
	    DigitNode* tail = nullptr;
	    DigitNode* curr = other.head;
	    while (curr) {
	        appendNode(head, tail, curr->digit);
	        curr = curr->next;
	    }
	}

	void removeLeadingZeros() {
		if (head == nullptr) return;
		
		DigitNode* lastNonZero = nullptr;
		DigitNode* curr = head;
		
		// 1. 遍历一遍，记住最后一个不是 0 的节点在哪
		while (curr != nullptr) {
			if (curr->digit != 0) {
				lastNonZero = curr;
			}
			curr = curr->next;
		}
		
		// 2. 如果全是 0 (比如 000)，只保留第一个节点
		if (lastNonZero == nullptr) {
			curr = head->next;
			while (curr != nullptr) {
				DigitNode* nextNode = curr->next;
				delete curr;
				curr = nextNode;
			}
			head->next = nullptr;
			head->digit = 0; 
		} 
		// 3. 正常情况，把 lastNonZero 后面的 0 全删了
		else {
			curr = lastNonZero->next;
			while (curr != nullptr) {
				DigitNode* nextNode = curr->next;
				delete curr;
				curr = nextNode;
			}
			lastNonZero->next = nullptr;
		}
	}
	};