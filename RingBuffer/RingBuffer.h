#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <mutex>

/*
 Creates a circular buffer array of size. 
 Data should be added to the head and pulled from the tail
 as the buffer maintains limited history.
 Supports multi-threading.
 Example:
    RingBuffer<int> buffer(10);
    for(uint16_t i=0;i<20;i++){
        int tmp = i;
        buffer.Put(tmp);
        if(buffer.IsFull())
            printf("Pulling %i from the buffer\n", buffer.Pull());
    }
 */
template <class T>
class RingBuffer
{
    public:
        RingBuffer(size_t size) :
            buf_(std::unique_ptr<T[]>(new T[size])),
            max_size_(size)
        {
            //empty constructor
        }
        
        /*
         Resets the position of the head and tail.
         Sets the size of the buffer to zero
         */
        void Reset()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            head_ = tail_;
            full_ = false;
        }

        /*
         Checks if the buffer is empty
         Return: True if the buffer is empty 
         */
        bool IsEmpty() const
        {
            //if head and tail are equal, we are empty
            return (!full_ && (head_ == tail_));
        }

        /*
         Checks if the buffer is full
         Return: true if the buffer is full
         */
        bool IsFull() const
        {
            //If tail is ahead the head by 1, we are full
            return full_;
        }

        /*
         Get the max allowable size of the buffer
         Return: Max size of buffer before overflow
         */
        size_t GetMaxSize() const
        {
            return max_size_;
        }

        /*
         Get the current size of the buffer
         Return: The size of the buffer between head and tail
         */
        size_t GetSize() const
        {
            size_t size = max_size_;

            if(!full_)
            {
                if(head_ >= tail_)
                {
                    size = head_ - tail_;
                }
                else
                {
                    size = max_size_ + head_ - tail_;
                }
            }

            return size;
        }

        /*
         Adds an item to the head of the buffer.
         Increments the head and/or tail appropriately.
         @param item: The templated item you want to add to the buffer
         */
        void Put(T item)
        {
            std::lock_guard<std::mutex> lock(mutex_);

            buf_[head_] = item;

            if(full_)
            {
                tail_ = (tail_ + 1) % max_size_;
            }

            head_ = (head_ + 1) % max_size_;

            full_ = head_ == tail_;
        }

        /*
         Pull the last item at the tail of the buffer
         Return: Tail item of the buffer or an empty item 
         */
        T Pull()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(IsEmpty())
            {
                return T();
            }
            //Read data and advance the tail (we now have a free space)
            auto val = buf_[tail_];
            full_ = false;
            tail_ = (tail_ + 1) % max_size_;

            return val;
        }

    private:
        /* Thread controller */
        std::mutex mutex_;

        /* Buffer of data stored */
        std::unique_ptr<T[]> buf_;

        /* Circular buffer data management variables */
        size_t head_ = 0;           // The head of the buffer, where data is added
        size_t tail_ = 0;           // The tail of the buffer, where data will be pulled from once the buffer is too large
        const size_t max_size_;     // The max size of the buffer
        bool full_ = 0;             // If the buffer is full or not
};

#endif