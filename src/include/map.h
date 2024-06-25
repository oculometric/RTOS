#pragma once

// TODO: map

template <typename K, typename V>
class Map
{
    struct MapNode
    {
        K key;
        V value;
        MapNode<K, V>* last;
        MapNode<K, V>* next;
    };

};