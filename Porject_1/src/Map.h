#ifndef MAP_H
#define MAP_H

#include "TreeNode.h"


template<typename KeyType,typename ValueType>
class Map{
public:

    virtual ~Map(){}

    virtual void insert(const KeyType& key,const ValueType& value)=0;

    virtual ValueType* get(const KeyType& key)=0;

    /*
    根据key设置对应的value为新value，如果key不存在，会创建一个新的节点
    */
    virtual void set(const KeyType& key,const ValueType& value)=0;

    /*根据index找到目标的value*/
    virtual std::pair<KeyType&,ValueType&> findByIndex(const int index)=0;

    /*根据key返回value，如果value不存在，会添加一个默认的key-value键并返回*/
    virtual ValueType& operator[](const KeyType& key)=0;

    /*删除对应key-value对*/
    virtual void erase(const KeyType& key)=0;
    /*是否为空*/
    virtual bool empty() const=0;

    /*清除所有项*/
    virtual void clear()=0;

    /*得到对应key的node的index，不存在，返回-1*/
    virtual int indexOf(const KeyType& key) const=0;

    /*得到大小*/
    virtual int size() const=0;

    virtual std::string toString(int indent) const =0;

    virtual void print(std::string indent = "") const=0;
};


#endif /* EF7720CE_8FA7_44FE_9955_AD29A1654545 */
