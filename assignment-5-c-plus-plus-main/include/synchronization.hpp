#ifndef HW5_SYNCHRONIZATION_HPP
#define HW5_SYNCHRONIZATION_HPP

#include "perfEvent.hpp"
#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstring>
#include <immintrin.h>
#include <iostream>
#include <sys/mman.h>

// How to install:
// https://askubuntu.com/questions/1170054/install-newest-tbb-thread-building-blocks-on-ubuntu-18-04
#include <tbb/tbb.h>

using namespace std;

template <class T> struct ListBasedSetNoSync {
    static constexpr bool synchronized = false;
    static constexpr char name[] = "nosync";

    struct Entry {
        T key;
        Entry* next;
    };

    Entry staticHead;
    Entry staticTail;

    ListBasedSetNoSync() {
        staticHead.key = std::numeric_limits<T>::min();
        staticHead.next = &staticTail;
        staticTail.key = std::numeric_limits<T>::max();
        staticTail.next = nullptr;
    }

    bool contains(T k) {
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k)
            curr = curr->next;
        // Check if this element is the searched key and return the result
        return (curr->key == k);
    }

    void insert(T k) {
        // Start inserting a new element from the head
        Entry* pred = &staticHead;
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred = curr;
            curr = curr->next;
        }

        // As we have a set, there is nothing to do if the key is already
        // contained
        if (curr->key == k)
            return;

        // Create a new list element for the new key
        Entry* n = new Entry{k, curr};
        // Insert the element in the list
        pred->next = n;
    }

    void remove(T k) {
        Entry* pred = &staticHead;
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred = curr;
            curr = curr->next;
        }

        // If the element has the searched key, remove it
        if (curr->key == k) {
            pred->next = curr->next;
            // ignore reclamation for now
        }
    }
};

// Defines all required functions for the synchronized implementations
template <class T> struct ListBasedSetSync {
    static constexpr bool synchronized = true;

    virtual bool contains(T k) = 0;
    virtual void insert(T k) = 0;
    virtual void remove(T k) = 0;
};

// class M defines the used mutex, you can use the locks provided in tbb
template <class T, class M>
struct ListBasedSetCoarseLock : virtual public ListBasedSetSync<T> {
    // ToDo: implement ListBasedSetNoSync using Coarse-Grained Locking
    // To implement it you can copy ListBasedSetNoSync and extend it for the
    // needed locking.
    static constexpr char name[] = "coarse";

    struct Entry {
        T key;
        Entry* next;
    };

    Entry staticHead;
    Entry staticTail;
    M mutex;

    ListBasedSetCoarseLock() {
        staticHead.key = std::numeric_limits<T>::min();
        staticHead.next = &staticTail;
        staticTail.key = std::numeric_limits<T>::max();
        staticTail.next = nullptr;
    }

    bool contains(T k) {
        mutex.lock();
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k)
            curr = curr->next;
        // Check if this element is the searched key and return the result

        mutex.unlock();
        return (curr->key == k);
    }

    void insert(T k) {
        mutex.lock();
        // Start inserting a new element from the head
        Entry* pred = &staticHead;
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred = curr;
            curr = curr->next;
        }

        // As we have a set, there is nothing to do if the key is already
        // contained
        if (curr->key == k) {
            mutex.unlock();
            return;
        }

        // Create a new list element for the new key
        Entry* n = new Entry{k, curr};
        // Insert the element in the list
        pred->next = n;
        mutex.unlock();
    }

    void remove(T k) {
        mutex.lock();
        Entry* pred = &staticHead;
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred = curr;
            curr = curr->next;
        }

        // If the element has the searched key, remove it
        if (curr->key == k) {
            pred->next = curr->next;
            // ignore reclamation for now
        }
        mutex.unlock();
    }
};

// class M defines the used mutex
template <class T, class M>
struct ListBasedSetCoarseLockRW : virtual public ListBasedSetSync<T> {
    // ToDo: implement ListBasedSetNoSync using Coarse-Grained Locking with
    // read/write lock
    static constexpr char name[] = "coarseRW";

    struct Entry {
        T key;
        Entry* next;
    };

    Entry staticHead;
    Entry staticTail;
    M mutex;

    ListBasedSetCoarseLockRW() {
        staticHead.key = std::numeric_limits<T>::min();
        staticHead.next = &staticTail;
        staticTail.key = std::numeric_limits<T>::max();
        staticTail.next = nullptr;
    }

    bool contains(T k) {
        typename M::scoped_lock lock(mutex, false);
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k)
            curr = curr->next;
        // Check if this element is the searched key and return the result
        return (curr->key == k);
    }

    void insert(T k) {
        typename M::scoped_lock lock(mutex, true);
        // Start inserting a new element from the head
        Entry* pred = &staticHead;
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred = curr;
            curr = curr->next;
        }

        // As we have a set, there is nothing to do if the key is already
        // contained
        if (curr->key == k) {
            return;
        }

        // Create a new list element for the new key
        Entry* n = new Entry{k, curr};
        // Insert the element in the list
        pred->next = n;
    }

    void remove(T k) {
        typename M::scoped_lock lock(mutex, true);
        Entry* pred = &staticHead;
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred = curr;
            curr = curr->next;
        }

        // If the element has the searched key, remove it
        if (curr->key == k) {
            pred->next = curr->next;
            // ignore reclamation for now
        }
    }
};

// class M defines the used mutex
template <class T, class M>
struct ListBasedSetLockCoupling : virtual public ListBasedSetSync<T> {
    // ToDo: implement ListBasedSetNoSync using Lock Coupling
    static constexpr char name[] = "lockCoupling";

    struct Entry {
        T key;
        Entry* next;
        M mutex;
    };

    Entry staticHead;
    Entry staticTail;

    ListBasedSetLockCoupling() {
        staticHead.key = std::numeric_limits<T>::min();
        staticHead.next = &staticTail;
        staticTail.key = std::numeric_limits<T>::max();
        staticTail.next = nullptr;
    }

    bool contains(T k) {
        staticHead.mutex.lock();

        staticHead.next->mutex.lock();
        staticHead.mutex.unlock();
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            curr->next->mutex.lock();
            curr->mutex.unlock();
            curr = curr->next;
        }
        // Check if this element is the searched key and return the result
        const bool result = (curr->key == k);
        curr->mutex.unlock();
        return result;
    }

    void insert(T k) {
        // Start inserting a new element from the head
        staticHead.mutex.lock();
        Entry* pred = &staticHead;

        staticHead.next->mutex.lock();
        Entry* curr = staticHead.next;

        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred->mutex.unlock();
            pred = curr;

            curr->next->mutex.lock();
            curr = curr->next;
        }

        // As we have a set, there is nothing to do if the key is already
        // contained
        if (curr->key == k) {
            pred->mutex.unlock();
            curr->mutex.unlock();
            return;
        }

        // Create a new list element for the new key
        Entry* n = new Entry{k, curr};
        // Insert the element in the list
        pred->next = n;

        pred->mutex.unlock();
        curr->mutex.unlock();

        return;
    }

    void remove(T k) {
        staticHead.mutex.lock();
        Entry* pred = &staticHead;

        staticHead.next->mutex.lock();
        Entry* curr = staticHead.next;
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred->mutex.unlock();
            pred = curr;

            curr->next->mutex.lock();
            curr = curr->next;
        }

        // If the element has the searched key, remove it
        if (curr->key == k) {
            pred->next = curr->next;
            // ignore reclamation for now
        }

        pred->mutex.unlock();
        curr->mutex.unlock();
        return;
    }
};

// class M defines the used mutex
template <class T, class M>
struct ListBasedSetLockCouplingRW : virtual public ListBasedSetSync<T> {
    // ToDo: implement ListBasedSetNoSync using Lock Coupling with read/write
    // locks
    static constexpr char name[] = "lockCouplingRW";

    struct Entry {
        T key;
        Entry* next;
        M mutex;
    };

    Entry staticHead;
    Entry staticTail;

    ListBasedSetLockCouplingRW() {
        staticHead.key = std::numeric_limits<T>::min();
        staticHead.next = &staticTail;
        staticTail.key = std::numeric_limits<T>::max();
        staticTail.next = nullptr;
    }

    bool contains(T k) {
        staticHead.mutex.lock_shared();
        Entry* curr = staticHead.next;
        curr->mutex.lock_shared();
        staticHead.mutex.unlock_shared();
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            curr->next->mutex.lock_shared();
            curr->mutex.unlock_shared();
            curr = curr->next;
        }
        // Check if this element is the searched key and return the result
        curr->mutex.unlock_shared();
        return (curr->key == k);
    }

    void insert(T k) {
        // Start inserting a new element from the head
        Entry* pred = &staticHead;
        pred->mutex.lock();
        Entry* curr = staticHead.next;
        curr->mutex.lock();
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred->mutex.unlock();
            pred = curr;
            curr = curr->next;
            curr->mutex.lock();
        }

        // As we have a set, there is nothing to do if the key is already
        // contained
        if (curr->key == k) {
            pred->mutex.unlock();
            curr->mutex.unlock();
            return;
        }

        // Create a new list element for the new key
        Entry* n = new Entry{k, curr};
        // Insert the element in the list
        pred->next = n;

        pred->mutex.unlock();
        curr->mutex.unlock();
    }

    void remove(T k) {
        Entry* pred = &staticHead;
        pred->mutex.lock();
        Entry* curr = staticHead.next;
        curr->mutex.lock();
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred->mutex.unlock();
            pred = curr;
            curr = curr->next;
            curr->mutex.lock();
        }

        // If the element has the searched key, remove it
        if (curr->key == k) {
            pred->next = curr->next;
            // ignore reclamation for now
        }

        pred->mutex.unlock();
        curr->mutex.unlock();
    }
};

static size_t MAX_ATTEMPTS = 10;

// class M defines the used mutex
template <class T, class M>
struct ListBasedSetOptimistic : virtual public ListBasedSetSync<T> {
    // ToDo: implement ListBasedSetNoSync using Optimistic Locking
    static constexpr char name[] = "optimistic";

    struct Entry {
        T key;
        Entry* next;
        M mutex;
    };

    Entry staticHead;
    Entry staticTail;

    ListBasedSetOptimistic() {
        staticHead.key = std::numeric_limits<T>::min();
        staticHead.next = &staticTail;
        staticTail.key = std::numeric_limits<T>::max();
        staticTail.next = nullptr;
    }

    bool contains(T k) {
        // firt try optimistic
        for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
            Entry* curr = staticHead.next;
            // Iterate over all elements whose key is smaller, we stop at the
            // first element with key >= k
            while (curr->key < k)
                curr = curr->next;

            bool curr_try = curr->mutex.try_lock();
            if (!curr_try) {
                continue;
            }

            // start to validate the result
            bool validate = false;
            Entry* validate_curr = staticHead.next;
            while (validate_curr->key < k)
                validate_curr = validate_curr->next;
            if (curr->key == validate_curr->key &&
                curr->next == validate_curr->next) {
                validate = true;
            }

            if (!validate) {
                curr->mutex.unlock();
                continue;
            }
            curr->mutex.unlock();
            return (curr->key == k);
        }
        contains_pessimistic(k);
    }

    bool contains_pessimistic(T k) {
        staticHead.mutex.lock();
        Entry* curr = staticHead.next;
        curr->mutex.lock();
        staticHead.mutex.unlock();
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            curr->next->mutex.lock();
            curr->mutex.unlock();
            curr = curr->next;
        }
        // Check if this element is the searched key and return the result
        curr->mutex.unlock();
        return (curr->key == k);
    }

    void insert(T k) {
        for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
            // Start inserting a new element from the head
            Entry* pred = &staticHead;
            Entry* curr = staticHead.next;
            // Iterate over all elements whose key is smaller, we stop at the
            // first element with key >= k
            while (curr->key < k) {
                pred = curr;
                curr = curr->next;
            }

            bool pred_try = pred->mutex.try_lock();
            bool curr_try = curr->mutex.try_lock();
            if (!pred_try || !curr_try) {
                if (pred_try) {
                    pred->mutex.unlock();
                }
                if (curr_try) {
                    curr->mutex.unlock();
                }
                continue;
            }

            // As we have a set, there is nothing to do if the key is already
            // contained
            if (curr->key == k) {
                pred->mutex.unlock();
                curr->mutex.unlock();
                return;
            }

            // start to validate the result
            bool validate = false;
            Entry* validate_pred = &staticHead;
            Entry* validate_curr = staticHead.next;
            while (validate_curr->key < k) {
                validate_pred = validate_curr;
                validate_curr = validate_curr->next;
            }

            if (curr->key == validate_curr->key &&
                pred->key == validate_pred->key &&
                curr->next == validate_curr->next) {
                validate = true;
            }

            if (!validate) {
                pred->mutex.unlock();
                curr->mutex.unlock();
                continue;
            }

            // Create a new list element for the new key
            Entry* n = new Entry{k, curr};
            // Insert the element in the list
            pred->next = n;

            pred->mutex.unlock();
            curr->mutex.unlock();
            return;
        }
        insert_pessimistic(k);
    }

    void insert_pessimistic(T k) {
        // Start inserting a new element from the head
        Entry* pred = &staticHead;
        pred->mutex.lock();
        Entry* curr = staticHead.next;
        curr->mutex.lock();
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred->mutex.unlock();
            pred = curr;
            curr = curr->next;
            curr->mutex.lock();
        }

        // As we have a set, there is nothing to do if the key is already
        // contained
        if (curr->key == k) {
            pred->mutex.unlock();
            curr->mutex.unlock();
            return;
        }

        // Create a new list element for the new key
        Entry* n = new Entry{k, curr};
        // Insert the element in the list
        pred->next = n;

        pred->mutex.unlock();
        curr->mutex.unlock();
    }

    void remove(T k) {
        for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
            // Start inserting a new element from the head
            Entry* pred = &staticHead;
            Entry* curr = staticHead.next;
            // Iterate over all elements whose key is smaller, we stop at the
            // first element with key >= k
            while (curr->key < k) {
                pred = curr;
                curr = curr->next;
            }

            bool pred_try = pred->mutex.try_lock();
            bool curr_try = curr->mutex.try_lock();
            if (!pred_try || !curr_try) {
                if (pred_try) {
                    pred->mutex.unlock();
                }
                if (curr_try) {
                    curr->mutex.unlock();
                }
                continue;
            }

            // start to validate the result
            bool validate = false;
            Entry* validate_pred = &staticHead;
            Entry* validate_curr = staticHead.next;
            while (validate_curr->key < k) {
                validate_pred = validate_curr;
                validate_curr = validate_curr->next;
            }

            if (curr->key == validate_curr->key &&
                pred->key == validate_pred->key &&
                curr->next == validate_curr->next) {
                validate = true;
            }

            if (!validate) {
                pred->mutex.unlock();
                curr->mutex.unlock();
                continue;
            }

            // If the element has the searched key, remove it
            if (curr->key == k) {
                pred->next = curr->next;
            }

            pred->mutex.unlock();
            curr->mutex.unlock();
            return;
        }
        remove_pessimistic(k);
    }

    void remove_pessimistic(T k) {
        Entry* pred = &staticHead;
        pred->mutex.lock();
        Entry* curr = staticHead.next;
        curr->mutex.lock();
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred->mutex.unlock();
            pred = curr;
            curr = curr->next;
            curr->mutex.lock();
        }

        // If the element has the searched key, remove it
        if (curr->key == k) {
            pred->next = curr->next;
            // ignore reclamation for now
        }

        pred->mutex.unlock();
        curr->mutex.unlock();
    }
};

// class M defines the used mutex
template <class T, class M>
struct ListBasedSetOptimisticLockCoupling : virtual public ListBasedSetSync<T> {
    // ToDo: implement ListBasedSetNoSync using Optimistic Lock Coupling
    static constexpr char name[] = "optimisticLockCoupling";

    struct Entry {
        T key;
        Entry* next;
        M mutex;

        // recored the version of the current Entry node
        uint64_t version = 0;
    };

    Entry staticHead;
    Entry staticTail;

    ListBasedSetOptimisticLockCoupling() {
        staticHead.key = std::numeric_limits<T>::min();
        staticHead.next = &staticTail;
        staticTail.key = std::numeric_limits<T>::max();
        staticTail.next = nullptr;
    }

    bool contains(T k) {
        // firt try optimistic
        for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
            Entry* curr = staticHead.next;
            // Iterate over all elements whose key is smaller, we stop at the
            // first element with key >= k
            uint64_t pre_version = curr->version;
            while (curr->key < k && curr->version == pre_version) {
                curr = curr->next;
                pre_version = curr->version;
            }

            // start to validate the result
            if (pre_version == curr->version) {
                return (curr->key == k);
            }
        }
        contains_pessimistic(k);
    }

    bool contains_pessimistic(T k) {
        staticHead.mutex.lock();
        Entry* curr = staticHead.next;
        curr->mutex.lock();
        staticHead.mutex.unlock();
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            curr->next->mutex.lock();
            curr->mutex.unlock();
            curr = curr->next;
        }
        // Check if this element is the searched key and return the result
        curr->mutex.unlock();
        return (curr->key == k);
    }

    void insert(T k) {
        for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
            // Start inserting a new element from the head
            Entry* pred = &staticHead;
            Entry* curr = staticHead.next;
            uint64_t pre_pred_version = pred->version;
            uint64_t pre_curr_version = curr->version;
            // Iterate over all elements whose key is smaller, we stop at the
            // first element with key >= k
            while (curr->key < k && pred->version == pre_pred_version &&
                   curr->version == pre_curr_version) {
                pred = curr;
                curr = curr->next;
                pre_pred_version = pred->version;
                pre_curr_version = curr->version;
            }

            bool pred_try = pred->mutex.try_lock();
            bool curr_try = curr->mutex.try_lock();
            if (!pred_try || !curr_try || pred->version != pre_pred_version ||
                curr->version != pre_curr_version) {
                if (pred_try) {
                    pred->mutex.unlock();
                }
                if (curr_try) {
                    curr->mutex.unlock();
                }
                continue;
            }

            // As we have a set, there is nothing to do if the key is already
            // contained
            if (curr->key == k) {
                pred->mutex.unlock();
                curr->mutex.unlock();
                return;
            }

            // Create a new list element for the new key
            Entry* n = new Entry{k, curr};
            // Insert the element in the list
            pred->next = n;

            // update version
            pred->version++;
            curr->version++;

            pred->mutex.unlock();
            curr->mutex.unlock();
            return;
        }
        insert_pessimistic(k);
    }

    void insert_pessimistic(T k) {
        // Start inserting a new element from the head
        Entry* pred = &staticHead;
        pred->mutex.lock();
        Entry* curr = staticHead.next;
        curr->mutex.lock();
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred->mutex.unlock();
            pred = curr;
            curr = curr->next;
            curr->mutex.lock();
        }

        // As we have a set, there is nothing to do if the key is already
        // contained
        if (curr->key == k) {
            pred->mutex.unlock();
            curr->mutex.unlock();
            return;
        }

        // Create a new list element for the new key
        Entry* n = new Entry{k, curr};
        // Insert the element in the list
        pred->next = n;

        pred->version++;
        curr->version++;

        pred->mutex.unlock();
        curr->mutex.unlock();
    }

    void remove(T k) {
        for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
            // Start inserting a new element from the head
            Entry* pred = &staticHead;
            Entry* curr = staticHead.next;
            uint64_t pre_pred_version = pred->version;
            uint64_t pre_curr_version = curr->version;
            // Iterate over all elements whose key is smaller, we stop at the
            // first element with key >= k
            while (curr->key < k && pred->version == pre_pred_version &&
                   curr->version == pre_curr_version) {
                pred = curr;
                curr = curr->next;
                pre_pred_version = pred->version;
                pre_curr_version = curr->version;
            }

            bool pred_try = pred->mutex.try_lock();
            bool curr_try = curr->mutex.try_lock();
            if (!pred_try || !curr_try || pred->version != pre_pred_version ||
                curr->version != pre_curr_version) {
                if (pred_try) {
                    pred->mutex.unlock();
                }
                if (curr_try) {
                    curr->mutex.unlock();
                }
                continue;
            }

            // If the element has the searched key, remove it
            if (curr->key == k) {
                pred->next = curr->next;
            }

            // update version
            pred->version++;
            curr->version++;

            pred->mutex.unlock();
            curr->mutex.unlock();
            return;
        }
        remove_pessimistic(k);
    }

    void remove_pessimistic(T k) {
        Entry* pred = &staticHead;
        pred->mutex.lock();
        Entry* curr = staticHead.next;
        curr->mutex.lock();
        // Iterate over all elements whose key is smaller, we stop at the first
        // element with key >= k
        while (curr->key < k) {
            pred->mutex.unlock();
            pred = curr;
            curr = curr->next;
            curr->mutex.lock();
        }

        // If the element has the searched key, remove it
        if (curr->key == k) {
            pred->next = curr->next;
            // ignore reclamation for now
        }
        pred->version++;
        curr->version++;
        pred->mutex.unlock();
        curr->mutex.unlock();
    }
};

#endif // HW5_SYNCHRONIZATION_HPP
