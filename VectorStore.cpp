#include "VectorStore.h"

// HELPER FUNCTIONS
void merge(Pair arr[], int left, int mid, int right, bool cosine)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Pair* L = new Pair[n1];
    Pair* R = new Pair[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        bool cond;
        if (cosine) {
            cond = (L[i].value > R[j].value);  // cosine → lớn hơn thì ưu tiên
        } else {
            cond = (L[i].value < R[j].value);  // distance → nhỏ hơn thì ưu tiên
        }

        if (cond) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(Pair arr[], int left, int right, bool cosine)
{
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, cosine);
        mergeSort(arr, mid + 1, right, cosine);
        merge(arr, left, mid, right, cosine);
    }
}

// ----------------- ArrayList Implementation -----------------

template <class T>
void ArrayList<T>::ensureCapacity(int cap)
{
    if (capacity >= cap) return;
    else
    {
        int newcap = floor(capacity*1.5);
        T *newdata = new T[newcap];
        for(int i=0; i<count; i++)
        {
            newdata[i] = data[i];
        }

        delete[] data;
        data = newdata;
        capacity = newcap;
    }
    ensureCapacity(cap);
}

template <class T>
ArrayList<T>::ArrayList(int initCapacity)
{
    capacity = initCapacity;
    count = 0;
    data = new T[capacity];
}

template <class T>
ArrayList<T>::ArrayList(const ArrayList<T>& other)
{
    // TODO
    capacity = other.capacity;
    count = other.count;
    data = new T[capacity];

    for (int i=0; i<count; i++)
    {
        data[i] = other.data[i];
    }

}   

template <class T>
ArrayList<T>::~ArrayList() {
    // TODO
    delete[] data;
}
template <class T>
ArrayList<T> &ArrayList<T>::operator=(const ArrayList<T>& other)
{
    if (this == &other) return *this;

    delete[] data;

    capacity = other.capacity;
    count = other.count;
    data = new T[capacity];
    for(int i=0; i<count; i++)
    {
        data[i] = other.data[i];
    }

    return *this;

}
template <class T>
void ArrayList<T>::add(T e)
{
    ensureCapacity(count + 1);
    data[count++] = e;

}
template <class T>
void ArrayList<T>::add(int index, T e)
{
    if (index < 0 || index > count)
    {
        throw std::out_of_range("Index is invalid!");
    }

    ensureCapacity(count + 1);
    for(int i=count; i>index; i--)
    {
        data[i] = data[i - 1];
    }
    data[index] = e;
    count++;
}
template <class T>
T ArrayList<T>::removeAt(int index)
{
    if (index < 0 || index >= count)
    {
        throw std::out_of_range("Index s invalid!");
    }
    T temp = data[index];
    for(int i=index; i<count - 1; i++)
    {
        data[i] = data[i + 1];
    }
    count--;
    return temp;
}
template <class T>
bool ArrayList<T>::empty() const
{
    return count == 0;
}
template <class T>
int ArrayList<T>::size() const
{
    return count;
}
template <class T>
void ArrayList<T>::clear()
{
    delete[] data;
    count = 0;
    capacity = 10;
    data = new T[capacity];
}
template <class T>
T &ArrayList<T>::get(int index) 
{
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    return data[index];
}
template <class T>
T &ArrayList<T>::get(int index) const
{
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    return data[index];
}
template <class T>
void ArrayList<T>::set(int index, T e)
{
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    data[index] = e;
}
template <class T>
int ArrayList<T>::indexOf(T item) const
{
    for(int i=0; i<count; i++)
    {
        if (item == data[i]) return i;
    }
    return -1;
}
template <class T>
bool ArrayList<T>::contains(T item) const
{
    for(int i=0; i<count; i++)
    {
        if (item == data[i]) return true;
    }
    return false;
}
template <class T>
string ArrayList<T>::toString(string (*item2str)(T&) ) const
{
    stringstream ss;
    ss << '[';

    for(int i=0; i<count; i++)
    {
        // if (item2str) ss << item2str(data[i]);
        // else return "[]";

        if (item2str) ss << item2str(data[i]);

        else ss << data[i];
        
        if (i < count - 1) ss << ", ";
    }
    ss << ']';
    return ss.str();
}
template <class T>
typename ArrayList<T>:: Iterator ArrayList<T>::begin()
{
    return Iterator(this,0);
}
template <class T>
typename ArrayList<T>:: Iterator ArrayList<T>::end()
{
    return Iterator(this,count);
}
// TODO: implement other methods of ArrayList



// ----------------- Iterator of ArrayList Implementation -----------------
template <class T>
ArrayList<T>::Iterator::Iterator(ArrayList<T>* pList, int index) {
    // TODO
    if (pList == nullptr)
    {
        if (index != 0) throw std::out_of_range("Index is invalid!");
    }
    else
    {
        if (index < 0 || index > pList->count) throw std::out_of_range("Index is invalid!");
    }

    this->pList = pList;
    this->cursor = index;
}
template <class T>
typename ArrayList<T>::Iterator&::ArrayList<T>::Iterator::operator=(const Iterator &other)
{
    if (this == &other) return *this;
    this->pList->data = new T;
    *this->pList->data = *(other.pList->data);
    this->cursor = other.cursor;
    return *this;
}
template <class T>
T& ArrayList<T>::Iterator::operator*()
{
    if (pList == nullptr || cursor < 0 || cursor >= pList->count) throw std::out_of_range("Iterator is out of range!");
    return pList->data[cursor];
}
template <class T>
bool ArrayList<T>::Iterator::operator!=(const Iterator &other) const
{
    return (this->cursor != other.cursor || this->pList != other.pList);
}
template <class T>
typename ArrayList<T>::Iterator&::ArrayList<T>::Iterator::operator++()
{
    if (cursor >= pList->count) throw std::out_of_range("Iterator cannot advance past end!");
    ++cursor;
    return *this;
}
template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator++(int)
{
    if (cursor >= pList->count) throw std::out_of_range("Iterator cannot advance past end!");
    Iterator old = *this;
    cursor++;
    return old;
}
template <class T>
typename ArrayList<T>::Iterator&::ArrayList<T>::Iterator::operator--()
{
    if (cursor <= 0) throw std::out_of_range("Iterator cannot move before begin!");
    --cursor;
    return *this;
}
template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator--(int)
{
    if (cursor <= 0) throw std::out_of_range("Iterator cannot move before begin!");
    if (cursor == pList->count) cursor = pList->count - 1;
    Iterator old = *this;
    cursor--;
    return old;
}

// TODO: implement other methods of ArrayList::Iterator



// ----------------- SinglyLinkedList Implementation -----------------
template <class T>
SinglyLinkedList<T>::SinglyLinkedList() {
    // TODO
    head = nullptr;
    tail = nullptr;
    count = 0;
}   

template <class T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    // TODO
    Node *curr = head;
    while(curr != nullptr)
    {
        Node *nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    count = 0;
}
template <class T>
void SinglyLinkedList<T>::add(T e)
{
    Node *newNode = new Node(e);
    if (head == nullptr)
    {
        head = newNode;
        tail = head;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
    count++;
}   
template <class T>
void SinglyLinkedList<T>::add(int index, T e)
{
    if (index < 0 || index > count) throw std::out_of_range("Index is invalid!");
    Node *newNode = new Node(e);

    if (index == 0)
    {
        newNode->next = head;
        head = newNode;
        if (count == 0) tail = newNode;
    }
    else if (index == count)
    {
        tail->next = newNode;
        tail = newNode;
    }
    else
    {
        Node *prev = head;
        for(int i=0; i<index - 1; i++)
        {
            prev = prev->next;
        }
        newNode->next = prev->next;
        prev->next = newNode;
    }
    count++;
}
template <class T>
T SinglyLinkedList<T>::removeAt(int index)
{
    if (head == nullptr || index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    T val; //value of index
    if (index  == 0)
    {
        Node *temp = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        val = temp->data;
        delete temp;
    }
    else
    {
        Node *temp = head;
        for (int i=0; i<index - 1; i++)
        {
            temp = temp->next;
        }
        Node *temp2 = temp->next;
        temp->next = temp2->next;
        if (temp2 == tail) tail = temp;
        val = temp2->data;
        delete temp2;
    }
    count--;
    return val;
}
template <class T>
bool SinglyLinkedList<T>::empty() const
{
    return count == 0;
}
template <class T>
int SinglyLinkedList<T>::size() const
{
    return count;
}
template <class T>
bool SinglyLinkedList<T>::removeItem(T item) {
    Node* curr = head;
    Node* prev = nullptr;

    while (curr != nullptr)
    {
        if (curr->data == item)
        {
            if (prev == nullptr)
            {
                head = curr->next;
                if (head == nullptr) tail = nullptr;
            } 
            else
            {
                prev->next = curr->next;
                if (curr == tail) tail = prev;
            }
            delete curr;
            count--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}
template <class T>
void SinglyLinkedList<T>::clear()
{
    Node *curr = head;
    while(curr != nullptr)
    {
        Node *temp = curr->next;
        delete curr;
        curr = temp;
    }
    head = tail = nullptr;
    count = 0;
}
template <class T>
T &SinglyLinkedList<T>::get(int index) const
{
    if (head == nullptr || index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    Node *curr = head;
    int pos = 0;
    while(curr != nullptr)
    {
        if (index == pos) return curr->data;
        curr = curr->next;
        pos++;
    }

    return tail->data;
}
template <class T>
T &SinglyLinkedList<T>::get(int index)
{
    if (head == nullptr || index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    Node *curr = head;
    int pos = 0;
    while(curr != nullptr)
    {
        if (index == pos) return curr->data;
        curr = curr->next;
        pos++;
    }

    return tail->data;
}
template <class T>
int SinglyLinkedList<T>::indexOf(T item) const
{
    Node *curr = head;
    int pos = 0;
    while(curr != nullptr)
    {
        if (curr->data == item) return pos;
        curr = curr->next;
        pos++;
    }
    return -1;
}
template <class T>
bool SinglyLinkedList<T>::contains(T item)const
{
    Node *curr = head;
    while(curr != nullptr)
    {
        if (curr->data == item ) return true;
        curr = curr->next;
    }
    return false;
}
// template <class T>
// string SinglyLinkedList<T>::toString(string (*item2str)(T&)) const
// {
//     stringstream ss;

//     Node *curr = head;
//     if (item2str)
//     {
//         while(curr != nullptr)
//         {
//             if (curr == head || curr == tail)
//             {
//                 ss << '[' << item2str(curr->data) << ']';
//             }
//             ss << '[' << item2str(curr->data) << ']' << "->";
//             curr = curr->next;
//         }
//     }
//     // else return "";

//     else
//     {
//         while(curr != nullptr)
//         {
//             if (curr == head || curr == tail) ss << '[' << curr->data << ']';

//             ss << '[' << curr->data << ']' << "->";

//             curr = curr->next;
//         }
//     }

//     return ss.str();
// }

template <class T>
string SinglyLinkedList<T>::toString(string (*item2str)(T&)) const
{
    if (head == nullptr) return "[]";

    stringstream ss;
    Node* curr = head;

    while (curr != nullptr)
    {
        if (item2str) 
            ss << '[' << item2str(curr->data) << ']';
        else
            ss << '[' << curr->data << ']';

        if (curr->next != nullptr) ss << "->";
        curr = curr->next;
    }

    return ss.str();
}

template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::begin()
{
    return Iterator(head);
}
template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::end()
{
    return Iterator(tail->next);
}
template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::begin() const
{
    return Iterator(head);
}
template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::end() const
{
    return Iterator(tail->next);
}
// TODO: implement other methods of SinglyLinkedList



// ----------------- Iterator of SinglyLinkedList Implementation -----------------
template <class T>
SinglyLinkedList<T>::Iterator::Iterator(Node* node) {
    // TODO
    this->current = node;
}
template <class T>
typename SinglyLinkedList<T>::Iterator &SinglyLinkedList<T>::Iterator::operator=(const SinglyLinkedList<T>::Iterator &other)
{
    if (this != &other)
    {
        this->current = other.current;
    }
    return *this;
}
template <class  T>
T &SinglyLinkedList<T>::Iterator::operator*()
{
    if (this->current == nullptr) throw std::out_of_range("Iterator is out of range!");
    return this->current->data;
}
template <class T>
bool SinglyLinkedList<T>::Iterator::operator!=(const SinglyLinkedList<T>::Iterator &other) const
{
    return this->current != other.current;
}
template <class T>
typename SinglyLinkedList<T>::Iterator &SinglyLinkedList<T>::Iterator::operator++()
{
    if (current == nullptr) throw std::out_of_range("Iterator cannot advance past end!");
    current = current->next;
    return *this;
}
template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::Iterator::operator++(int)
{
    if (current == nullptr) throw std::out_of_range("Iterator cannot advance past end!");
    Iterator old = *this;
    current = current->next;
    return old;
}

// TODO: implement other methods of SinglyLinkedList::Iterator

// ----------------- VectorStore Implementation -----------------

VectorStore::VectorStore(int dimension, EmbedFn embeddingFunction) {
    // TODO
    this->dimension = dimension;
    this->embeddingFunction = embeddingFunction;
    this->count = 0;
}
VectorStore::VectorRecord::VectorRecord(int id, const string &rawText, SinglyLinkedList<float> *vector)
{
    this->id = id + 1;
    this->rawText = rawText;
    this->rawLength = rawText.size();
    this->vector = vector;
}
int VectorStore::size() const
{
    return this->count;
}
bool VectorStore::empty() const
{
    return count == 0;
}
void VectorStore::clear()
{
    for(int i=0; i<this->records.size(); i++)
    {
        VectorRecord *temp = this->records.get(i);
        if(temp->vector != nullptr) delete temp->vector;
        delete temp;
    }
    this->records.clear();
    count = 0;
}
SinglyLinkedList<float> *VectorStore::preprocessing(string rawText)
{
    SinglyLinkedList<float> *vec = embeddingFunction(rawText);
    int temp = vec->size();

    while(temp > this->dimension)
    {
        vec->removeAt(temp - 1);
        temp--;
    }
    while(temp < this->dimension)
    {
        vec->add(0.0f);
        temp++;
    }
    return vec;
}
void VectorStore::addText(string rawText)
{
    SinglyLinkedList<float> *temp = preprocessing(rawText);
    VectorRecord *newrec = new VectorRecord(count, rawText, temp);
    this->records.add(newrec);
    count++;
}
SinglyLinkedList<float> &VectorStore::getVector(int index)
{
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    return *(records.get(index)->vector);
}
string VectorStore::getRawText(int index) const
{
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    return (records.get(index)->rawText);
}
int VectorStore::getId(int index) const
{
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    return (records.get(index)->id);
}
bool VectorStore::removeAt(int index)
{
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    VectorRecord *temp = records.get(index);
    delete temp->vector;
    temp->vector = nullptr;
    delete temp;
    records.removeAt(index);
    count--;
    return true;
}
bool VectorStore::updateText(int index, string newRawText)
{
    if (index < 0 || index >= count) throw std::out_of_range("Index is invalid!");
    VectorRecord *rec = records.get(index);
    rec->rawText = newRawText;
    rec->rawLength = newRawText.size();
    delete rec->vector;
    rec->vector = preprocessing(newRawText);
    return true;
}
void VectorStore::setEmbeddingFunction(VectorStore::EmbedFn newEmbeddingFunction)
{
    this->embeddingFunction = newEmbeddingFunction;
}
void VectorStore::forEach(void (*action)(SinglyLinkedList<float> &, int, std::string &))
{
    for(auto it : records)
    {
        action(*it->vector, it->id, it->rawText);
    }
}
// double VectorStore::cosineSimilarity(const SinglyLinkedList<float> &v1, const SinglyLinkedList<float> &v2) const
// {
//     auto *node1 = v1.getHead();
//     auto *node2 = v2.getHead();
//     double dot = 0.0, A = 0.0, B = 0.0;

//     while(node1 != nullptr && node2 != nullptr)
//     {
//         double a = node1->data;
//         double b = node2->data;
//         dot += a * b;
//         A += a * a;
//         B += b * b;
//         node1 = node1->next;
//         node2 = node2->next;
//     }
//     return dot/(sqrt(A) * sqrt(B));
// }

// double VectorStore::cosineSimilarity(const SinglyLinkedList<float> &v1, const SinglyLinkedList<float> &v2) const{
//     SinglyLinkedList<float> tv1=v1;
//     SinglyLinkedList<float> tv2=v2;
//     double sumV1=0;
//     double sumV2=0;
//     double dotPro=0;
//     for(auto it1= tv1.begin(), it2=tv2.begin(); it1 != tv1.end() && it2 != tv2.end(); it2++, it1++){
//         dotPro += *it1 + *it2;
//     }
//     for(auto it= tv1.begin(); it != tv1.end();it++ ){
//         sumV1 += pow(*it,2);
//     }
//     for(auto it= tv2.begin(); it != tv2.end();it++ ){
//         sumV2 += pow(*it,2);
//     }
//     double cosine=0;
//     cosine= dotPro / sqrt(sumV1)*sqrt(sumV2);
//     return cosine;
// }
double VectorStore::cosineSimilarity(const SinglyLinkedList<float> &v1, const SinglyLinkedList<float> &v2) const
{
    double dot = 0.0, A = 0.0, B = 0.0;
    auto temp1 = v1.begin();
    auto temp2 = v2.begin();
    while(temp1 != v1.end() && temp2 != v2.end())
    {
        dot += *temp1 * *temp2;
        A += pow(*temp1,2);
        B += pow(*temp2,2);
        temp1++;
        temp2++;
    }
    if (temp1 != v1.end() || temp2 != v2.end()) return 0.0;
    if (A == 0.0 || B == 0.0) return 0.0;
    return dot/(sqrt(A) * sqrt(B));
}
double VectorStore::l1Distance(const SinglyLinkedList<float> &v1, const SinglyLinkedList<float> &v2) const
{
    auto it1 = v1.begin();
    auto it2 = v2.begin();
    double dis = 0.0;

    while(it1 != v1.end() && it2 != v2.end())
    {
        dis += fabs(*it1 - *it2);
        it1++;
        it2++;
    }
    while (it1 != v1.end()) {
        dis += fabs(*it1);
        ++it1;
    }
    while (it2 != v2.end()) {
        dis += fabs(*it2);
        ++it2;
    }
    return dis;
}
double VectorStore::l2Distance(const SinglyLinkedList<float> &v1, const SinglyLinkedList<float> &v2) const
{
    auto it1 = v1.begin();
    auto it2 = v2.begin();
    double temp = 0.0;
    while(it1 != v1.end() && it2 != v2.end())
    {
        temp += pow(*it1 - *it2, 2);
        it1++;
        it2++;
    }
    while (it1 != v1.end()) {
        temp += pow(*it1, 2);
        ++it1;
    }
    while (it2 != v2.end()) {
        temp += pow(*it2, 2);
        ++it2;
    }
    return sqrt(temp);
}
int VectorStore::findNearest(const SinglyLinkedList<float> &query, const std::string &metric) const
{
    if (metric != "cosine" && metric != "euclidean" && metric != "manhattan") throw invalid_metric();
    int bestindex = -1;
    double bestvalue = (metric == "cosine") ? (-1.0) : (1e9);

    for(int i=0; i<count; i++)
    {
        SinglyLinkedList<float> &vec = *(records.get(i)->vector);
        double res = 0.0;
        if (metric == "cosine")
        {
            res = cosineSimilarity(query,vec);
            if (res > bestvalue)
            {
                bestindex = i;
                bestvalue = res;
            }
        }
        if (metric == "euclidean")
        {
            res = l2Distance(query,vec);
            if (res < bestvalue)
            {
                bestvalue = res;
                bestindex = i;
            }
        }
        if (metric == "manhattan")
        {
            res = l1Distance(query,vec);
            if (res < bestvalue)
            {
                bestvalue = res;
                bestindex = i;
            }
        }
    }
    return bestindex;
}
int* VectorStore::topKNearest(const SinglyLinkedList<float>& query, int k, const std::string& metric) const {
    if (metric != "cosine" && metric != "euclidean" && metric != "manhattan")
        throw invalid_metric();
    if (k <= 0 || k > count)
        throw invalid_k_value();
    Pair* temp = new Pair[count];

    for (int i = 0; i < count; i++) {
        SinglyLinkedList<float>& vec = *(records.get(i)->vector);
        double res = 0.0;

        if (metric == "cosine")
            res = cosineSimilarity(query, vec);
        else if (metric == "euclidean")
            res = l2Distance(query, vec);
        else if (metric == "manhattan")
            res = l1Distance(query, vec);

        temp[i].value = res;
        temp[i].index = i;
    }

    bool cosine = (metric == "cosine");
    mergeSort(temp, 0, count - 1, cosine);

    int* result = new int[k];
    for (int i = 0; i < k; i++) {
        result[i] = temp[i].index;
    }

    delete[] temp;
    return result;
}

VectorStore::~VectorStore()
{
    for(int i=0; i<count; i++)
    {
        VectorRecord *rec = records.get(i);
        delete rec->vector;
        delete rec;
    }
    this->records.clear();
    count = 0;
}
// TODO: implement other methods of VectorStore


// Explicit template instantiation for char, string, int, double, float, and Point

template class ArrayList<char>;
template class ArrayList<string>;
template class ArrayList<int>;
template class ArrayList<double>;
template class ArrayList<float>;
template class ArrayList<Point>;

template class SinglyLinkedList<char>;
template class SinglyLinkedList<string>;
template class SinglyLinkedList<int>;
template class SinglyLinkedList<double>;
template class SinglyLinkedList<float>;
template class SinglyLinkedList<Point>;
