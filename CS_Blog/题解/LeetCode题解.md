# 5605.检查两个字符数组是否相等（字符串）

链接：[检查两个字符数组是否相等](https://leetcode-cn.com/problems/check-if-two-string-arrays-are-equivalent/)

## 解法一：拼接字符串

> 借助两个string类型变量，分别遍历两个vector，使用+=拼接字符串，最后进行比较

时间复杂度：O(N)

空间复杂度：O(1)

```cpp
class Solution {
public:
    bool arrayStringsAreEqual(vector<string>& word1, vector<string>& word2) {
        string a,b;
        for(auto w:word1) a += w;
        for(auto v:word2) b += v;
        return a == b;
    }
};
```

# 5606.具有给定数值的最小字符串（贪心）

链接：[具有给定数值的最小字符串](https://leetcode-cn.com/problems/smallest-string-with-a-given-numeric-value/)

## 解法一：逐位选择

> n位的数值范围为：n~26n
>
> 在每一位上面 从a开始至z 一个一个测试是否符合数值范围，符合就填入，为了符合字典序最小，所以需要从a开始，这样整个才能符合字典序最小

时间复杂度：O( 26 * N )

空间复杂度：O( N )

```cpp
class Solution {
public:
    string getSmallestString(int n, int k) {
        string ret;
        for(int i = 0, s = 0;i < n;i++) //遍历 1 - n 位置
        {
            for(int j = 1; j <= 26;j++) //枚举尝试26个字母
            {
                //尝试第j个字母时k还剩余多少，k - 当前字符数值 - 之前位置上的数值
                int last = k - s - j;   
                //尝试第i个位置时n还剩多少位，n - 当前位置（1）- 之前的位
                int m = n - i - 1;    
                //判断该位选择j字母是否合适 ，合适便退出，不合适继续寻找接下来的字母 
                if(last >= m && last <= 26 * m)
                {
                    ret += j + 'a' -1;
                    s += j;
                    break;
                }
            }
        }
        return ret;
    }
};
```

## 解法二：贪心法

> 将这n个字符全部填上a，再计算剩余数值够填多少个z，再计算剩余数值是哪一个字母
>
> 所以最终的字符串为  n1个a+最终剩余的那个字母+n2个z

时间复杂度：O( N )

空间复杂度：O( N )

```cpp
class Solution {
public:
    string getSmallestString(int n, int k) {
        string ret(n,'a');
        int count_z = (k - n) / 25;
        int befor_first_z = (k - n) % 25;
        ret[n - count_z - 1] = befor_first_z + 'a' ;
        for(int i = n - count_z; i< n;i++)
            ret[i] = 'z';
        return ret;    
     }
};
```

# 5607.生成平衡数组的方案数

链接：[生成平衡数组的方案数](https://leetcode-cn.com/problems/ways-to-make-a-fair-array/solution/)

## 解法一：前缀和+枚举

> 1. 求奇偶数前缀和数组
> 2. 从第0位尝试到第n位

时间复杂度：O( N )

空间复杂度：O( N )

```cpp
#include <vector>
class Solution {
public:
    int waysToMakeFair(vector<int>& nums) {
        int n = nums.size();
        vector<int> s1(n+1,0),s2(n+1,0);//s1是奇数前缀和数组 s2是偶数前缀和数组

        //处理奇偶前缀和数组
        for(int i = 1; i <= n; i++){
            s1[i] = s1[i - 1],s2[i] = s2[i - 1]; 
            if(i % 2) s1[i] += nums[i - 1]; //当前如果是奇数放入s1
            else s2[i] += nums[i - 1];      //偶数放入s2
        }

        int res = 0;
        for(int i = 0; i <= n; i++){
            //偶数和 = 第i个数前所有偶数和 + 第i个数后所有奇数和
            int odd = s1[i - 1] + s2[n] - s2[i];
            //奇数和 = 第i个数前所有奇数和 + 第i个数后所有偶数和
            int even = s2[i - 1] + s1[n] - s1[i];
            if(odd == even) res++;
        }
        return res;
    }
};
```

# 234.回文链表

链接：[回文链表](https://leetcode-cn.com/problems/palindrome-linked-list/)

## 解法一：转移法

> 用一个容器将链表中的值保存下来，再判断

时间复杂度：O( N )

空间复杂度：O( N )

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        //特殊情况判断
        if(head == NULL) return true;
        if(head->next == NULL)return true;
        //用于存储节点值
        vector<int> v;
        //遍历链表将值存在v中
        ListNode* temp = head;
        while(temp != NULL)
        {
            v.push_back(temp->val);
            temp = temp->next;
        }
        //左右对比判断是否为回文
        for(int i = 0; i <= v.size()/2;i++)
            if(v[i] != v[v.size() - 1 - i]) return false;
        return true;
    }
};
```

# 面试题02.07.链表相交

 链接：[链表相交](https://leetcode-cn.com/problems/intersection-of-two-linked-lists-lcci/)

## 解法一：查表法

> 1. 将一个链表中的所有节点放入set容器中
> 2. 遍历另外一个链表，查看结点是否在容器中出现过
> 3. 如果出现，则返回该结点；

时间复杂度：O( N ^ 2)

空间复杂度：O ( N )

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        //判断边界条件
        if(headA == NULL || headB == NULL) return NULL;
        //将其中一个链表放入set容器中
        set<ListNode*> s;
        while(headA != NULL)
        {
            s.insert(headA );
            headA = headA->next;
        }
        //查找另外一个链表中的结点是否在容器中出现过
        while(headB != NULL)
        {
            if(s.find(headB) != s.end() )
            {
                break;
            }
            headB = headB->next;
        }
        return headB;    
    }
};
```

# 1290.二进制链表转整数

链接：[二进制链表转整数](https://leetcode-cn.com/problems/convert-binary-number-in-a-linked-list-to-integer/)

## 解法一：遍历累加法

> 遍历链表并累加和

时间复杂度：O( N )

空间复杂度：O( 1 )

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    int getDecimalValue(ListNode* head) {
        //边界条件判断
        if(head == NULL) return 0;
        if(head->next == NULL) return head->val;

        int ans = 0;
        ListNode* temp = head;
        while(temp != NULL)
        {
            ans = ans*2 + temp->val;
            temp = temp->next;
        }
        return ans;
    }
};
```

# 148.排序链表

链接：[排序链表](https://leetcode-cn.com/problems/sort-list/)

## 解法一：借助容器排序

> 将数据放入multiset容器中，再取出放入链表中

时间复杂度：取决于multiset底层排序复杂度

空间复杂度：O( N )

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if(head == NULL || head->next == NULL) return head;
        multiset<int> ms;
        ListNode* temp = head;
        while(temp != NULL)
        {
            ms.insert(temp->val);
            temp = temp->next;
        }
        temp = head;
        auto i = ms.begin();
        while(temp != NULL)
        {
            temp->val = (*i);
            temp = temp->next;
            i++;
        }
        return head;
    }
};
```

# 61.旋转链表

链接：[旋转链表](https://leetcode-cn.com/problems/rotate-list/)

## 解法一：借助容器排序

> 将数字放进容器中旋转后赋值回去

时间复杂度：O( N )

空间复杂度：O( N )

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if(head == NULL || head->next == NULL) return head;

        vector<int> v;
        ListNode* temp = head;
        int length = ListLength(temp);
        temp = head;
        k %= length;

        if(k == 0 ) return head;
        
        //定位结点
        int count = length - k;
        while(count)
        {
            temp = temp->next;
            count--;
        }
        //插入结点后面的值
        while(temp)
        {
            v.push_back(temp->val);
            temp = temp->next;
        }
        //插入结点前面的值
        temp = head;
        count = k;
        while(temp && k)
        {
            v.push_back(temp->val);
            temp = temp->next;
            count--;
        }
        
        //赋值回去
        temp = head;
        for(int i = 0; temp && i < length; i++)
        {
            temp->val = v[i];
            temp = temp->next;
        }
        
        return head;
    }
private:
    int ListLength(ListNode* temp)
    {
        int ans = 0;
        while(temp)
        {
            temp = temp->next;
            ans++;
        }
        return ans;
    }
};
```

# 83.删除排序链表中的重复元素

链接：[删除排序链表中的重复元素](https://leetcode-cn.com/problems/remove-duplicates-from-sorted-list/)

## 解法一：双指针法

时间复杂度：O(N)

空间复杂度：O(1)

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if(head == NULL || head->next == NULL) return head;
        ListNode* temp = head;
        while(temp->next != NULL)
        {
            if(temp->val == temp->next->val)
            {
                ListNode* nextNode = temp->next;
                if(nextNode->next != NULL)
                {
                    nextNode = nextNode->next;
                    temp->next = nextNode;
                }
                else
                {
                    temp->next = NULL;
                    break;
                }
                continue;
            }
            temp = temp->next;
        }
        return head;
    }
};
```

# 面试题04.03.特定深度节点链表

链接：[特定深度结点链表](https://leetcode-cn.com/problems/list-of-depth-lcci/)

## 解法一：层序遍历

> 层序遍历

时间复杂度：O( N )

空间复杂度：O( N )

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    vector<ListNode*> listOfDepth(TreeNode* tree) {
        vector<ListNode*> vL;
        if (tree == NULL) return vL;
        queue<TreeNode*> q;
        TreeNode* temp = tree;
        q.push(temp);
        while (!q.empty())
        {
            ListNode* head = new ListNode(0);
            ListNode* tail = head;
            int size = q.size();
            while (size)
            {
                TreeNode* front = q.front();
                q.pop();

                tail->next = new ListNode(front->val);
                tail = tail->next;

                size--;
                if(front->left != NULL)
                    q.push(front->left);
                if(front->right != NULL)
                    q.push(front->right);
            }
            tail->next = NULL;
            ListNode* delNode = head;
            head = head->next;
            delete delNode;
            delNode = NULL;
            vL.push_back(head);
        }
        return vL;
    }
};
```

# 328.奇偶链表

链接：[奇偶链表](https://leetcode-cn.com/problems/odd-even-linked-list/)

## 解法一：双指针法

> 使用两个指针分别指向奇偶结点进行链接奇偶链表

时间复杂度：O( N )

空间复杂度：O( 1 )

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if(head == nullptr || head->next == nullptr) return head;

        ListNode* oddNode = head;//奇数指针
        ListNode* evenNode = head->next;//偶数指针
        ListNode* evenHeadNode = evenNode;//偶链表头指针
        
        //链接奇偶链表
        while(oddNode && evenNode && oddNode->next && evenNode->next )
        {
            oddNode->next = evenNode->next;
            oddNode = evenNode->next;
            if(oddNode->next)
            {
                evenNode->next = oddNode->next;
                evenNode = oddNode->next;
            }
        }
        
        //将奇偶链表链接
        if(oddNode->next == nullptr)
        {
            evenNode->next = nullptr;
            oddNode->next = evenHeadNode;
        }
        if(evenNode->next == nullptr)
        {
            oddNode->next = evenHeadNode;
        }
        
        return head;
    }
};
```

# 109.有序链表转换二叉搜索树

链接：[有序链表转换二叉搜索树](https://leetcode-cn.com/problems/convert-sorted-list-to-binary-search-tree/)

## 解法一：分治+中序遍历

- 思路

![image.png](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210303104257.png)

时间复杂度：O( N )

空间复杂度：O (logN)

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if(head == nullptr) return nullptr;
        int len = getLength(head);
        return buildTree(head, 0, len - 1);
    }
private:
    int getLength(ListNode* temp)
    {
        int ans = 0;
        while(temp)
        {
            temp = temp->next;
            ans++;
        }
        return ans;
    }
    TreeNode* buildTree(ListNode*& node,int left,int right)
    {
        if(left > right)
            return nullptr;
        int mid = (left + right + 1) / 2;
        TreeNode* temp = new TreeNode();
        temp->left = buildTree(node,left,mid - 1);
        temp->val = node->val;
        node = node->next;
        temp->right = buildTree(node,mid + 1,right);
        return temp;
    }
};
```

# 338.比特位计数

链接：[338. 比特位计数](https://leetcode-cn.com/problems/counting-bits/)

## 解法一：位运算

> 逐个数字进行统计二进制位上 1 的个数

```cpp
class Solution {
public:
    vector<int> countBits(int num) {
        vector<int> v;
        if(num < 0) return v;
        for(int i = 0; i <= num; i++)
            v.push_back(countOne(i));
        return v;
    }
private:
    int countOne(const int& i)
    {
        int count = 0;
        int temp = i;
        while(temp)
        {
            if(temp & 1) count++;
            temp >>= 1;
        }
        return count;
    }
};
```

# 523. 连续的子数组和

链接：[523. 连续的子数组和](https://leetcode-cn.com/problems/continuous-subarray-sum/)

## 解法一：前缀和 + 哈希

构建前缀和数组，并将前缀和数组中所有元素对 k 取余

一边构建前缀和取余数组，一边向哈希表中存储 余数-坐标 映射关系

若 当前余数已经在哈希表中存在，找到上一次出现的坐标，判断两次坐标距离大于等于 2 ，则为 true
$$
sum[j] - sum[j-1] = n * k 
$$


```c++
bool checkSubarraySum(vector<int>& nums, int k) 
{
    int n = nums.size();
    if(n < 2) return false;

    unordered_map<int, int> m;
    m[0] = -1;  //用于表示空前缀和的情况，或者前m个恰好为k的倍数时的左端点
    int s = 0;	// 用于存储前缀和中间值
    for(int i = 0; i < n ; i++)
    {
        s = (s + nums[i]) % k;
        if(m.count(s))	// 该前缀和余数 之前已经存在了
        {
            int prvIdx = m[s];
            if(i - prvIdx >= 2) 
                return true;
        }
        else m[s] = i;		//将余数存在哈希表中
    }
    return false;
}
```



# 525. 连续数组

链接：[525. 连续数组](https://leetcode-cn.com/problems/contiguous-array/)

## 解法一：前缀和+哈希

遍历数组，使用一个变量 `s` 保存前缀和，若数组值为 `1` 则 `s++` 反之 `s--` 

若当前 `s` 值不存在于哈希表中，则将 `s` 以 `s - i` 的形式保存在哈希表中

若当前 `s` 值存在于哈希表中，说明已经经过了 `i - hash[s]` 个`具有相等个数的01串`

```cpp
int findMaxLength(vector<int>& v) 
{
    int n = v.size();
    int s = 0, ans = 0;
    unordered_map<int, int> m;
    m[s] = -1;
    for(int i = 0; i < n; i++)
    {
        if(v[i]) s++;
        else s--;
        if(m.count(s))  ans = max(ans, i - m[s]);
        else m[s] = i;
    }
    return ans;
}
```





# 160. 相交链表

链接：[160. 相交链表](https://leetcode-cn.com/problems/intersection-of-two-linked-lists/)

## 解法一：哈希

将一个链表存在哈希表中，再遍历另外一个链表，如果在哈希表中判断该结点是否出现即可

```cpp
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) 
{
    unordered_set<ListNode*> s;
    while(headA)
    {
        s.insert(headA);
        headA = headA->next;
    }
    while(headB)
    {
        if(s.count(headB))
            return headB;
        headB = headB->next;
    }
    return headB;
}
```





# 494. 目标和

链接：[494. 目标和](https://leetcode-cn.com/problems/target-sum/)

## 解法一：DFS+回溯

对每一位进行`+`操作和`-`操作，当遍历到最后一位时，对比 `um`与 `target`的值是否相等

```cpp
int count = 0;
int findTargetSumWays(vector<int>& nums, int target) {
    DFS(nums, target, 0, 0);
    return count;
}
void DFS(vector<int>& nums, int target, int index, int sum)
{
    if(index == nums.size())
    {
        if(sum == target)
            count ++;
    }
    else
    {
        DFS(nums, target, index+1, sum + nums[index]);
        DFS(nums, target, index+1, sum - nums[index]);
    }
}
```

