# 1. AVL树的概念

* AVL树又叫平衡二叉树，是俄罗斯的数学家G.M.Adelson-Velskii和E.M.Landis在1962年发明了一种数据结构
* AVL树的特点：向二叉搜索树中插入新结点后，保证每个结点的左右子树**高度之差的绝对值不超过1**
* AVL树的性质：
  * 左右子树高度之差(简称平衡因子)的绝对值不超过1
  * 它的左右子树都是AVL树
* AVL树的性能：如果它有n个结点，其高度可以保持在O(logN)，搜索时间复杂度为O(logN)

![image-20210220114218189](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210220114218.png)

# 2. AVL树节点的定义

```cpp
// AVL树结点结构
template<class T>
struct AVLTreeNode
{
	AVLTreeNode<T>* left;
	AVLTreeNode<T>* right;
	AVLTreeNode<T>* parent;
	T data;
	int _bf; //表示该结点的平衡因子

	AVLTreeNode(const T& x = T()) :left(nullptr), right(nullptr), parent(nullptr), data(x), _bf(0){}
};

// 假设：AVL树中的值是唯一的
template<class T>
class AVLTree
{
	typedef AVLTreeNode<T> Node;
public:
	AVLTree() : _root(nullptr){}
	~AVLTree(){ Destroy(_root); }
	bool insert(const T& data);
	void InOrder();
	bool IsBalanceTree();

private:
	AVLTreeNode<T>* _root;	// 指向根结点的指针

	void InOrder(Node* root);
	void Destroy(Node*& root);
	void RightRotation(Node* parent);
	void LeftRotation(Node* parent);
	void Left_Right_DoubleRotation(Node* parent);
	void Right_Left_DoubleRotation(Node* parent);
	int _Height(Node* root);
	bool _IsBalanceTree(Node* root);
};
```

* left指针：指向该结点的左孩子
* right指针：指向该结点的右孩子
* parent指针：指向该结点的父节点
* data：存储关键值
* _bf：该结点的平衡因子，该文中定义：平衡因子 = 右子树高度 - 左子树高度

# 3. AVL树节点的插入

1. 仿照BST插入结点规则寻找插入位置，同时保存插入位置父节点
2. 找到插入位置后，插入该结点
3. 从插入结点的父节点开始，自下而上调整平衡因子
4. 调整平衡因子时：若平衡因子的绝对值等于2，就需要对该结点的子树进行旋转

```cpp
bool insert(const T& data) // 保证元素值不重复
{
    // 1. 按照二叉搜索树规则插入新节点
    // 1.1 树为空
    if (nullptr == _root)
    {
        _root = new Node(data);
        return true;
    }

    // 1.2 树不为空
    // 1.2.1 寻找二叉搜索树的插入位置，并保存其双亲
    Node* cur = _root;
    Node* parent = nullptr;
    while (cur)
    {
        parent = cur; //保存父节点
        if (data < cur->data)
            cur = cur->left;
        else if (data > cur->data)
            cur = cur->right;
        else
            return false;
    }

    // 1.2.2 插入结点
    cur = new Node(data);
    if (data < parent->data)
        parent->left = cur;
    else
        parent->right = cur;

    cur->parent = parent;

    // 2. 新节点插入后，一直向上调整父节点平衡因子
    while (parent)
    {
        // 2.1 更新父节点
        if (cur == parent->left)  // parent右子树高度没有变，而左子树高度增加了一层
            parent->_bf--;
        else
            parent->_bf++;

        // 2.2 向上迭代
        if (0 == parent->_bf)
            break;
        else if (-1 == parent->_bf || 1 == parent->_bf)
        {
            // 以parent为根的子树的高度增加了一层，继续向上一层更新
            cur = parent;
            parent = cur->parent;
        }
        else
        {
            //  parent的平衡因子为 2或-2，需要对parent为根的二叉树进行旋转处理
            //	parent与cur的_bf的正负：同号用单旋，异号用双旋
            if (2 == parent->_bf) // 右子树高
            {
                if (1 == cur->_bf) // 左单旋
                    // 左单旋
                    LeftRotation(parent);
                else  // 右左双旋
                    Right_Left_DoubleRotation(parent);
            }
            else // 左子树高
            {
                //parent的平衡因子为-2
                if (-1 == cur->_bf) //右单旋
                    RightRotation(parent);
                else  // 左右双旋
                    Left_Right_DoubleRotation(parent);
            }
            break;
        }// end if - else if - else
    } // end while
    return true;
}
```



# 4. AVL树的旋转

`代码图解四种旋转`

> 右单旋

![image-20210220113232599](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210220113239.png)

```cpp
void RightRotation(Node* parent) // 右单旋：新节点插入到较高左子树的左子树上
{
    Node* subL = parent->left; // 父节点的左孩子节点
    Node* subLR = subL->right; // 父节点的左孩子节点的右孩子节点

    /*
		右单旋：
		1. 父节点的左指针指向subLR，实现将parent的右子树下降一层
		2. subL的右指针指向parent，实现将parent的左子树上升一层
	*/

    // 右单旋 1
    parent->left = subLR;
    if (subLR) // 避免左单支的场景：subLR是nullptr
        subLR->parent = parent;

    // 右单旋 2
    subL->right = parent;
    // 因为parent可能是某个节点的子树，因此在更新parent的双亲前必须先将其之前的双亲记录
    Node* pparent = parent->parent; // 可能parent是非根结点，需要记录parent的父结点
    parent->parent = subL;
    subL->parent = pparent;

    // parent可能是根节点：需要修改_root
    // parent也可能是一棵子树: 需要修改pparent的左/右指针域的指向
    if (nullptr == pparent) // 若是根结点，修改树的root指向
    {
        // 旋转之前parent是根节点
        _root = subL;
    }
    else // 若是非根结点，修改parent父节点的指针指向
    {
        // parent是某个节点的子树
        if (parent == pparent->left) // 判断parent是父节点的哪个孩子节点
            pparent->left = subL;
        else
            pparent->right = subL;
    }
    // 将parent和subL的平衡因子更新为0
    parent->_bf = subL->_bf = 0;
}
```

> 左单旋

![image-20210220113325952](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210220113326.png)

```cpp
void LeftRotation(Node* parent)
{
    Node* subR = parent->right; // 父节点的右孩子节点
    Node* subRL = subR->left; // 父节点的右孩子节点的左孩子节点

    /*
		左单旋：
		1. parent节点的右指针指向subRL，实现将parent的右子树下降一层
		2. subR的左指针指向parent，实现将parent的左子树上升一层
		*/
    // 左单旋 1
    parent->right = subRL;
    if (subRL) // 避免：右单支, 防止subRL是nullptr
        subRL->parent = parent;
    // 左单旋 2
    subR->left = parent;
    Node* pparent = parent->parent; // 需要更新parent和subR的双亲
    parent->parent = subR;
    subR->parent = pparent;

    // 旋转之前：parent可能是根结点，可能是非根结点
    // parent是根节点：修改_root的指向
    // parent是子树：修改原parent左||右指针域的指向
    if (nullptr == pparent) // 若是根结点，修改树的root指向
    {
        _root = subR;
    }
    else  // 若是非根结点，修改parent父节点的指针指向
    {
        if (parent == pparent->left)  // 判断parent是父节点的哪个孩子节点
            pparent->left = subR;
        else
            pparent->right = subR;
    }

    // 将parent和subR的平衡因子更新为0
    parent->_bf = subR->_bf = 0;
}
```



> 左-右 双旋

![image-20210220113352464](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210220113352.png)

```cpp
void Left_Right_DoubleRotation(Node* parent)
{
    // 旋转之前必须要保存subLR, 因为旋转完成之后需要根据subLR之前平衡因的情况来跟新旋转完成后
    // 部分节点的平衡因子
    Node* subL = parent->left;
    Node* subLR = subL->right;
    int bf = subLR->_bf;

    LeftRotation(parent->left);
    RightRotation(parent);

    if (1 == bf)
        subL->_bf = -1;
    else if (-1 == bf)
        parent->_bf = 1;
}
```



> 右-左 双旋

![image-20210220113410852](https://raw.githubusercontent.com/gongruiyang/BlogImage/main/img/20210220113410.png)

```cpp
void Right_Left_DoubleRotation(Node* parent)
{
    Node* subR = parent->right;
    Node* subRL = subR->left;
    int bf = subRL->_bf;

    RightRotation(parent->right);
    LeftRotation(parent);

    if (1 == bf)
        parent->_bf = -1;
    else if (-1 == bf)
        subR->_bf = 1;
}
```

# 5. AVL树的验证

* 判断该树是否为AVL树

```cpp
int _Height(Node* root)
{
    if (nullptr == root)	// 函数出口：遍历到nullptr结点
        return 0;

    int leftHeight = _Height(root->left);	// 记录左子树的高度
    int rightHegiht = _Height(root->right);	// 记录右子树的高度
    return leftHeight > rightHegiht ? leftHeight + 1 : rightHegiht + 1;	// 返回最高的子树加上本节点的个数
}
bool _IsBalanceTree(Node* root)
{
    // 空树也是AVL树
    if (nullptr == root)
        return true;

    // 计算root根节点的平衡因子是否正确
    int leftHeight = _Height(root->left);
    int rightHegiht = _Height(root->right);
    int bf = rightHegiht - leftHeight;
    if (abs(bf) > 1 || bf != root->_bf)
    {
        cout << root->data << " 节点的平衡因子有问题" << endl;
        return false;
    }
    return _IsBalanceTree(root->left) && _IsBalanceTree(root->right);
}
bool IsBalanceTree()
{
    return _IsBalanceTree(_root);
}
```

# 6. AVL树的销毁

* 采用后序遍历法逐个结点销毁

```cpp
void Destroy(Node*& root) // 二叉树后序遍历规则：后序销毁二叉树
{
    if (root)
    {
        Destroy(root->left);
        Destroy(root->right);
        delete root;
        root = nullptr;
    }
}
```

