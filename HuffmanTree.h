//
// Created by 韩磊 on 20/10/9.
//

#ifndef TEST10_HUFFMANTREE_H
#define TEST10_HUFFMANTREE_H

#include"Heap.h"

template<class T>
struct HuffmanTreeNode
{
    HuffmanTreeNode<T>* _left;
    HuffmanTreeNode<T>* _right;
    HuffmanTreeNode<T>* _parent;
    T _data;
    HuffmanTreeNode(const T& d)
            :_data(d)
            , _left(NULL)
            , _right(NULL)
            , _parent(NULL)
    {}

};
template<class T>
struct WeitLessThan
{
    bool operator()(HuffmanTreeNode<T>* _left, HuffmanTreeNode<T>* _right)
    {
        return _left->_data < _right->_data;
    }
};

template<class T>
class HuffmanTree
{
    typedef HuffmanTreeNode<T> Node;
private:
    Node* _root;
public:
    HuffmanTree()  //无参构造
            :_root(NULL)
    {
    }
    HuffmanTree(T* a, size_t n ,T invalid)         //传上来一个数组   //在文件压缩项目里面加上T类型的invalid非法值
    {
        //建堆
        Heap<Node*, WeitLessThan<T>> minHeap;
        for (size_t i = 0; i < n; i++)
        {
            if (a[i] != invalid)
            {
                minHeap.Push(new Node(a[i]));
            }
        }
        //建huffman树
        while (minHeap.Size()>1)
        {
            //选出权值最小的两个结点
            Node* left = minHeap.Top();
            minHeap.Pop();
            Node* right = minHeap.Top();
            minHeap.Pop();
            //构造父结点然后三点相链
            Node* parent = new Node(left->_data + right->_data);
            parent->_left = left;
            left->_parent = parent;                //parent指针
            parent->_right = right;
            right->_parent = parent;               //链接好之后再放回去比较 // 注意：放回去比较没啥特别的，要知道是放回到数组里面，链接好的不会产生什么影响
            minHeap.Push(parent);
        }
        _root = minHeap.Top();
    }

    Node* GetRoot()
    {
        return _root;
    }
};

#endif //TEST10_HUFFMANTREE_H
