//============================================================================
// Name        : Data structures and Algorithms
// Author      : Brian Toulouse
// Version     : 1.0
// Description : This program takes information from a csv file and loads them into diffrent data structures.
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include <vector>
#include "CSVparser.hpp"
using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 20000;
// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    return;
}

// ********************* Start Vector Class *************************
/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bids.push_back(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end)
{
    Bid temp;
    bool done = false;

    // sets mid to pivot
    int mid = begin + (end - begin) / 2;
    Bid pivot = bids.at(mid);
    int l = begin;
    int h = end;

    while (!done)
    {
        // Incrament l while bids[l] < pivot
        while (bids.at(l).title.compare(pivot.title) == -1)
        {
            ++l;
        }
        // decrement h while bids[h] > pivot
        while (pivot.title.compare(bids.at(h).title) == -1)
        {
            --h;
        }
        // checks for 1 or 0 elements remaning, if true, return h
        if (l >= h)
        {
            done = true;
        }
        // if false swap bids[l] with bids[h]
        else
        {
            temp = bids.at(l);
            bids.at(l) = bids.at(h);
            bids.at(h) = temp;

            ++l;
            --h;
        }
    }
    return h;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    // already sorted
    if (begin >= end)
    {
        return;
    }

    int pivot = partition(bids, begin, end);

    quickSort(bids, begin, pivot);
    quickSort(bids, pivot + 1, end);

    return;
}

/**
 * Perform a selection sort on bid title
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    int smallInd = 0;
    Bid temp;

    for (unsigned int i = 0; i < bids.size(); ++i)
    {
        smallInd = i;
        for (unsigned int j = i + 1; j < bids.size(); ++j)
        {
            if (bids.at(j).title.compare(bids.at(smallInd).title) == -1)
            {
                smallInd = j;
            }
        }
        temp = bids.at(i);
        bids.at(i) = bids.at(smallInd);
        bids.at(smallInd) = temp;
    }
}

// ************************** Start Binary Tree ***************************
struct Node {
    Bid bid;
    Node* left = nullptr;
    Node* right = nullptr;

    Node(Bid myBid) {
        this->bid = myBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);
    Node* minVal(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
}
/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // Implement inserting a bid into the tree
    if (root == nullptr)
    {
        root = new Node(bid);
    }
    else
    {
        this->addNode(root, bid);
    }
}

/**
 * Remove a bid
 */
Node* BinarySearchTree::removeNode(Node* root, string bidId)
{
    Node* temp;
    if (root == NULL)
    {
        return root;
    }

    if (bidId < root->bid.bidId)
    {
        root->left = removeNode(root->right, bidId);
    }
    else if (bidId > root->bid.bidId)
    {
        root->right = removeNode(root->right, bidId);
    }
    else
    {
        if (root->left == NULL)
        {
            temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = minVal(root->right);
        root->bid.bidId = temp->bid.bidId;
        root->right = removeNode(root->right, temp->bid.bidId);
    }
}

Node* BinarySearchTree::minVal(Node* node)
{
    Node* current = node;
    while (current->left != NULL)
    {
        current = current->left;
    }
    return current;
}
void BinarySearchTree::Remove(string bidId) {
    // Implement removing a bid from the tree
    removeNode(root, bidId);

}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // Implement searching the tree for a bid

    Bid bid;
    Node* current = root;

    while (current != nullptr)
    {
        if (current->bid.bidId.compare(bidId) == 0)
        {
            return current->bid;
        }
        if (bidId.compare(current->bid.bidId) < 0)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    // Implement inserting a bid into the tree
    if (node->bid.bidId.compare(bid.bidId) > 0)
    {
        if (node->left == nullptr)
        {
            node->left = new Node(bid);
        }
        else
        {
            this->addNode(node->left, bid);
        }
    }
    else
    {
        if (node->right == nullptr)
        {
            node->right = new Node(bid);
        }
        else
        {
            this->addNode(node->right, bid);
        }
    }
}
void BinarySearchTree::inOrder(Node* node) {
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    /*
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    */
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // push this bid to the end
            bst->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}
//============================================================================
// Hash Table class definition
//============================================================================

/**
* Define a class containing data members and methods to
* implement a hash table with chaining.
*/
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned key;
        Node* nextNodePtr;

        // constructor
        Node() {
            key = UINT_MAX;
            nextNodePtr = nullptr;
        }

        // Node initialized with a bid
        Node(Bid myBid) : Node() {
            bid = myBid;
        }

        Node(Bid myBid, unsigned newKey) : Node(myBid) {
            key = newKey;
        }
    };

    vector<Node> myNodes;

    unsigned setSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
* Default constructor
*/
HashTable::HashTable() {
    // Initialize the structures used to hold bids
    myNodes.resize(setSize);
}

HashTable::HashTable(unsigned size) {
    this->setSize = size;
    myNodes.resize(setSize);
}

/**
* Destructor
*/
HashTable::~HashTable() {
    // Implement logic to free storage when class is destroyed
    myNodes.erase(myNodes.begin());
}

/**
* Calculate the hash value of a given key.
* Note that key is specifically defined as
* unsigned int to prevent undefined results
* of a negative list index.
*
* @param key The key to hash
* @return The calculated hash
*/
unsigned int HashTable::hash(int key) {
    // Implement logic to calculate a hash value
    return key % setSize;
}

/**
* Insert a bid
*
* @param bid The bid to insert
*/
void HashTable::Insert(Bid bid) {
    // Implement logic to insert a bid
    unsigned key = hash(atoi(bid.bidId.c_str()));

    // search for node with the key value

    Node* prevNode = &(myNodes.at(key));

    if (prevNode == nullptr) {
        Node* nextNode = new Node(bid, key);
        myNodes.insert(myNodes.begin() + key, (*nextNode));
    }
    else {
        // if node is found
        if (prevNode->key == UINT_MAX) {
            prevNode->key = key;
            prevNode->bid = bid;
            prevNode->nextNodePtr = nullptr;
        }
        else {
            // if not found, find the next node available
            while (prevNode->nextNodePtr != nullptr) {
                prevNode = prevNode->nextNodePtr;
            }
        }
    }
}

/**
* Print all bids
*/
void HashTable::PrintAll() {
    // Implement logic to print all bids
    for (unsigned int i = 0; i < myNodes.size(); ++i) {
        displayBid(myNodes[i].bid);
    }
}

/**
* Remove a bid
*
* @param bidId The bid id to search for
*/
void HashTable::Remove(string bidId) {
    // Implement logic to remove a bid
    unsigned key = hash(atoi(bidId.c_str()));
    myNodes.erase(myNodes.begin() + key);
}

/**
* Search for the specified bidId
*
* @param bidId The bid id to search for
*/
Bid HashTable::Search(string bidId) {
    Bid bid;

    // Implement logic to search for and return a bid
    unsigned key = hash(atoi(bidId.c_str()));

    // search for node with the key value

    Node* node = &(myNodes.at(key));

    // search for node using key

    // if node is found by given key
    if (node != nullptr && node->key != UINT_MAX
        && node->bid.bidId.compare(bidId) == 0) {
        return node->bid;
    }

    // if there is no node with the key value
    if (node == nullptr || node->key == UINT_MAX) {
        return bid;
    }

    // traverse list to look for a mat h
    while (node != nullptr) {
        if (node->key != UINT_MAX && node->bid.bidId.compare(bidId) == 0) {
            return node->bid;
        }
        node = node->nextNodePtr;
    }

    return bid;
}
/**
* Load a CSV file containing bids into a container
*/
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}
/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}
/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, csvPath2, bidKey;

    csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
    csvPath2 = "eBid_Monthly_Sales.csv";
    // Define a vector to hold all the bids
    vector<Bid> bids;
    // Define a Binary Tree to hold all the bids
    BinarySearchTree* bst{};
    // Define a hash table to hold all the bids
    HashTable* bidTable{};
    Bid bid;
    // Define a timer variable
    clock_t ticks;

    int fileChoice = 0;
    int dataStructureChoice = 0;
    int choice = 0;

    while (dataStructureChoice != 9) {
        cout << "Select a Data Structure:" << endl;
        cout << "  1. Vector" << endl;
        cout << "  2. Binary Tree" << endl;
        cout << "  3. Hash Table" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> dataStructureChoice;

        switch (dataStructureChoice) {
        case 1:
            while (choice != 9) {
                cout << "Menu:" << endl;
                cout << "  1. Load Bids" << endl;
                cout << "  2. Display All Bids" << endl;
                cout << "  3. Selection Sort All Bids" << endl;
                cout << "  4. Quick Sort All Bids" << endl;
                cout << "  9. Return to main menu" << endl;
                cout << "Enter choice: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    // Initialize a timer variable before loading bids
                    ticks = clock();
                        while (fileChoice != 1 && fileChoice != 2) {
                            cout << "Enter 1 for the month of December file (170 items), 2 for the entire year (17,000 itmes) file: ";
                            cin >> fileChoice;
                            cout << endl;
                        }
                        if (fileChoice == 1) {
                            // Complete the method call to load the bids
                            bids = loadBids(csvPath);
                        }
                        else if (fileChoice == 2) {
                            // Complete the method call to load the bids
                            bids = loadBids(csvPath2);
                        }
                        fileChoice = 0;
                    cout << bids.size() << " bids read" << endl;

                    // Calculate elapsed time and display result
                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                    cout << "time: " << ticks << " clock ticks" << endl;
                    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                    break;

                case 2:
                    // Loop and display the bids read
                    for (unsigned int i = 0; i < bids.size(); ++i) {
                        displayBid(bids[i]);
                    }
                    cout << endl;

                    break;

                case 3:
                    ticks = clock();
                    selectionSort(bids);

                    // Calculate elapsed time and display result
                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                    cout << "time: " << ticks << " clock ticks" << endl;
                    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                    break;

                case 4:
                    ticks = clock();
                    quickSort(bids, 0, bids.size() - 1);

                    // Calculate elapsed time and display result
                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                    cout << "time: " << ticks << " clock ticks" << endl;
                    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                    break;
                default:
                    break;
                }
            }
            choice = 0;
            break;
        case 2:
            while (choice != 9) {
                cout << "Menu:" << endl;
                cout << "  1. Load Bids" << endl;
                cout << "  2. Display All Bids" << endl;
                cout << "  3. Find Bid" << endl;
                cout << "  4. Remove Bid" << endl;
                cout << "  9. Return to main menu" << endl;
                cout << "Enter choice: ";
                cin >> choice;

                switch (choice) {

                case 1:
                    bst = new BinarySearchTree();

                    // Initialize a timer variable before loading bids
                    ticks = clock();

                    while (fileChoice != 1 && fileChoice != 2) {
                        cout << "Enter 1 for the month of December file (170 items), 2 for the entire year (17,000 itmes) file: ";
                        cin >> fileChoice;
                        cout << endl;
                    }
                    if (fileChoice == 1) {
                        // Complete the method call to load the bids
                        loadBids(csvPath, bst);
                    }
                    else if (fileChoice == 2) {
                        // Complete the method call to load the bids
                        loadBids(csvPath2, bst);
                    }
                    fileChoice = 0;

                    //cout << bst->Size() << " bids read" << endl;

                    // Calculate elapsed time and display result
                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                    cout << "time: " << ticks << " clock ticks" << endl;
                    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                    break;

                case 2:
                    bst->InOrder();
                    break;

                case 3:
                    ticks = clock();
                    cout << "Enter Bid ID Ex: 98109" << endl;
                    cin >> bidKey;
                    bid = bst->Search(bidKey);

                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                    if (!bid.bidId.empty()) {
                        displayBid(bid);
                    }
                    else {
                        cout << "Bid Id " << bidKey << " not found." << endl;
                    }

                    cout << "time: " << ticks << " clock ticks" << endl;
                    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                    break;

                case 4:
                    cout << "Enter Bid ID Ex: 98109" << endl;
                    cin >> bidKey;
                    bst->Remove(bidKey);
                    break;
                }
            }
            choice = 0;
            break;
        case 3:
            cout << "Hash case" << endl;
            while (choice != 9) {
                cout << "Menu:" << endl;
                cout << " 1. Load Bids" << endl;
                cout << " 2. Display All Bids" << endl;
                cout << " 3. Find Bid" << endl;
                cout << " 4. Remove Bid" << endl;
                cout << " 9. Return to main menu" << endl;
                cout << "Enter choice: ";
                cin >> choice;

                switch (choice) {

                case 1:
                    bidTable = new HashTable();

                    // Initialize a timer variable before loading bids
                    ticks = clock();
                    while (fileChoice != 1 && fileChoice != 2) {
                        cout << "Enter 1 for the month of December file (170 items), 2 for the entire year (17,000 itmes) file: ";
                        cin >> fileChoice;
                        cout << endl;
                    }
                    if (fileChoice == 1) {
                        // Complete the method call to load the bids
                        loadBids(csvPath, bidTable);
                    }
                    else if (fileChoice == 2) {
                        // Complete the method call to load the bids
                        loadBids(csvPath2, bidTable);
                    }
                    fileChoice = 0;

                    // Calculate elapsed time and display result
                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                    cout << "time: " << ticks << " clock ticks" << endl;
                    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                    break;

                case 2:
                    bidTable->PrintAll();
                    break;

                case 3:
                    ticks = clock();
                    cout << "Enter Bid ID Ex: 98109" << endl;
                    cin >> bidKey;
                    bid = bidTable->Search(bidKey);

                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                    if (!bid.bidId.empty()) {
                        displayBid(bid);
                    }
                    else {
                        cout << "Bid Id " << bidKey << " not found." << endl;
                    }

                    cout << "time: " << ticks << " clock ticks" << endl;
                    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
                    break;

                case 4:
                    cout << "Enter Bid ID Ex: 98109" << endl;
                    cin >> bidKey;
                    bidTable->Remove(bidKey);
                    break;
                }
            }
            choice = 0;
            break;
        default:
            break;
        }
     }

    cout << "Good bye." << endl;
    return 0;
}
