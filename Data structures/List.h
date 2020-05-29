//
// Created by 3P on 4/6/2020.
//

#ifndef LAB5_LIST_H
#define LAB5_LIST_H
using namespace std;


template<class type>
class List {
protected:
    template<class T>
    class Node {
    private:
        T value;
        Node<T>* next;
        Node<T>* prev;

    public:
        Node() :prev(NULL), next(NULL)
        {

        }

        explicit Node(const T value, Node<T>* prev = NULL, Node<T>* next = NULL) :value(value), prev(prev), next(next)
        {

        }

        ~Node() = default;

        void setNext(Node<T>* newNext)
        {
            next = newNext;
        }

        void setPrev(Node<T>* newPrev)
        {
            prev = newPrev;
        }

        void setValue(const T newValue) {
            value = newValue;
        }

        T getValue() {
            return value;
        }

        Node<T>* getNext() {
            return next;
        }

        Node<T>* getPrev() {
            return prev;
        }
    };

    Node<type>* root, * last;

    unsigned long int length;

    Node<type>* getNodeByIndex(int index) {
        Node<type>* ans = root;
        for (int i = 0; i < index; i++) {
            ans = ans->getNext();
        }
        return ans;
    }

public:
    List() :root(NULL), last(NULL), length(0)
    {

    }

    List(const type& value) :root(NULL), last(NULL), length(0)
    {
        append(value);
    }

    List(const std::vector<type>& valuesVector) : root(NULL), last(NULL), length(0)
    {
        for (type value : valuesVector) {
            append(value);
        }
    }

    ~List() {
        while (length != 0) pop();
    }

    void append(const type& value) {
        if (root != NULL) {
            Node<type>* newNode = new Node<type>(value, last);
            last->setNext(newNode);
            last = newNode;
        }
        else {
            Node<type>* newNode = new Node<type>(value);
            root = newNode;
            last = newNode;
        }
        length++;
    }

    void insert(const type& value, int index = -1) {
        if(index < 0) index += length;
        try {
            if ((length > 0 && (index > length - 1 || index < 0)) || (length == 0 && (index < -1 || index > 0))) throw "Index out of range at function insert().\n";

            Node<type>* newNode = new Node<type>(value);

            if(length == 0){
                root = newNode;
                last = newNode;
            }
            else{
                auto next = getNodeByIndex(index);
                newNode->setNext(next);
                newNode->setPrev(next->getPrev());
                next->getPrev()->setNext(newNode);
                next->setPrev(newNode);
            }
            length++;
        }
        catch (const char* message) {
            std::cerr << message;
        }
    }

    void pop(int index = -1){
        try {
            if(length > 0){
                if (index < 0) index += length;
                if (index > length - 1 || index < 0) throw "Index out of range in pop().\n";
                if(length == 1){
                    delete root;
                    root = NULL;
                    last = NULL;
                }
                else{
                    Node<type> *victim = root;
                    for (int i = 0; i < index; i++) {
                        victim = victim->getNext();
                    }

                    if (victim->getPrev() == NULL) {
                        Node<type> *newRoot = victim->getNext();
                        newRoot->setPrev(NULL);
                        delete root;
                        root = newRoot;
                    }
                    else if (victim->getNext() == NULL){
                        Node<type>* newLast = victim->getPrev();
                        newLast->setNext(NULL);
                        delete last;
                        last = newLast;
                    }
                    else {
                        Node<type> *prev = victim->getPrev();
                        prev->setNext(victim->getNext());
                        Node<type> *next = victim->getNext();
                        next->setPrev(victim->getPrev());
                        delete victim;
                    }
                }
                length--;
            }
        }
        catch (const char* message) {
            std::cerr << message;
        }

    }

    unsigned long int len() {
        return length;
    }

    type operator[](const int& index)
    {
        try {
            if (index > length - 1 || index < 0) throw "Index out of range while using operator[]. You tried to get element with index ";
            Node<type>* output = root;
            for (int i = 0; i < index; i++) {
                output = output->getNext();
            }
            return output->getValue();
        }
        catch (const char* message) {
            std::cerr << message << index << ".\n";
        }
    }

    int find(const type& value) {
        int index = 0;
        for (Node<type>* it = root; it != NULL; it = it->getNext()) {
            if (it->getValue() == value) {
                return index;
            }
            index++;
        }
        return -1;
    }

    bool operator==(const List& list) {
        if (length != list.len()) return false;
        Node<type>* tmp = root;
        for (int i = 0; i < length; i++) {
            if (tmp->getValue() != list[i]) return false;
            tmp = tmp->getNext();
        }
        return true;
    }

    bool operator!=(const List& list) {
        if (*this == list) return false;
        return true;
    }

    type* currentCondition() {
        type* currcond = new type[length];
        Node<type>* tmp = root;
        for (int i = 0; i < length; i++) {
            currcond[i] = tmp->getValue();
            tmp = tmp->getNext();
        }
        //For chars:
        if (typeid(type).name() == typeid(char).name()) {
            currcond[this->length] = '\0';
        }

        return currcond;
    }

    List subList(const int& startPosition, int Length) {
        try {
            if (startPosition > length - 1 || startPosition < 0) throw "Index out of range at function subList(). You tried to start from position ";

            List* sublist = new List();
            if (Length > length - startPosition) Length = length - startPosition;
            Node<type>* tmp = getNodeByIndex(startPosition);

            for (int i = startPosition; i < Length + startPosition; i++) {
                sublist->append(tmp->getValue());
                tmp = tmp->getNext();
            }
            return *sublist;
        }
        catch (const char* message) {
            std::cerr << message << startPosition << " when min is 0 and max is " << length - 1 << ".\n";
        }
    }

    friend std::ostream& operator<< (std::ostream &out, List<type>& list){
        out << "[" << list[0];
        for(int i = 1; i < list.len(); i++){
            out << ", " << list[i];
        }
        out << "]";
        return out;
    }

//    iterator begin() { return &[0]; }
//    const_iterator begin() const { return &store[0]; }
//    iterator end() { return &store[size]; }
//    const_iterator end() const { return &store[size]; }
};


#endif //LAB5_LIST_H
