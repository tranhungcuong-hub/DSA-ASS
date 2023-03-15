#include <iostream>
#include <queue>

using namespace std;

template <class T>
class BTree
{
    class Node
    {
    public:
        T data;
        Node *left;
        Node *right;

        Node(T data = 0, Node *left = 0, Node *right = 0)
        {
            this->data = data;
            this->left = left;
            this->right = right;
        }
    };

public:
    BTree()
    {
        root = nullptr;
    }
    void insert(T *arr, int size);
    Node *insert(Node *root, queue<T> queue);
    void printIn();
    void printInorder(Node *root);
    void mirror();
    void printR2L(Node *root);

    Node *root;
};

template <class T>
void BTree<T>::insert(T *arr, int size)
{
    queue<T> queue;
    for (int i = 0; i < size; i++)
    {
        queue.push(arr[i]);
    }
    this->root = insert(this->root, queue);
}

template <class T>
typename BTree<T>::Node *BTree<T>::insert(Node *root, queue<T> queue)
{
    if (queue.empty())
        return nullptr;
    if (!root)
    {
        T val = queue.front();
        queue.pop();
        root = new Node(val);
        cout << root->data << endl;
    }
    root->left = insert(root->left, queue);
    root->right = insert(root->right, queue);

    return root;
}

template <class T>
void BTree<T>::printIn()
{
    printInorder(this->root);
}

template <class T>
void BTree<T>::printInorder(Node *root)
{
    if (root == nullptr)
    {
        return;
    }

    printInorder(root->left);

    cout << root->data << " ";

    printInorder(root->right);
}

template <class T>
void BTree<T>::mirror()
{
}

template <class T>
void BTree<T>::printR2L(Node *root)
{
}

int main()
{
    int arr[] = {10, 5, 1, 3, 2, 4};
    BTree<int> tree;
    tree.insert(arr, 6);
    tree.printIn();
    return 0;
}