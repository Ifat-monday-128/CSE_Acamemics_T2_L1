#include <bits/stdc++.h>
using namespace std;

using ll = unsigned long long;

const int size_init = 13;
const double highest_load = 0.50;
const double minimum_load = 0.25;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; 1LL * i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int nextPrime(int number) {
    number++;
    while (!isPrime(number)) number++;
    return number;
}

int previousPrime(int number) {
    while (!isPrime(number)) number--;
    return number;
}

// Hash1 from your base offline assignment.
ll hash1(const string& word) {
    ll hashValue = 5381;
    for (char ch : word) hashValue = hashValue * 41 + ch;
    return hashValue;
}

// Hash2 from your base offline assignment.
ll hash2(const string& word) {
    ll hashValue = 14695981039346656037ULL;
    for (char ch : word) {
        hashValue ^= (unsigned char)ch;
        hashValue *= 1099511628211ULL;
    }
    return hashValue;
}

// Used by Mock B so the same hash-table class can store integer keys.
ll integerHash(const int& value) {
    return (ll)(unsigned int)value;
}

// Position-weighted Hash1 used by the Section-B-style Mocks A and D.
ll positionHash(const string& word) {
    ll hashValue = 0;
    for (int i = 0; i < (int)word.size(); i++)
        hashValue += 1ULL * (i + 1) * (unsigned char)word[i];
    return hashValue % 13;
}

double weight_cal(const string& word) {
    int first = (unsigned char)word.front();
    int last = (unsigned char)word.back();
    return ((first + last) % 100) / 100.0;
}

enum Method { CHAINING, DOUBLE_HASHING, CUSTOM_PROBING };
enum slotState { EMPTY, OCCUPIED, DELETED };

/*
    Reusable base table from your offline solution.
    Extra public helpers were added only for the mock questions:
      - getCapacity(), getCount(), getLoadFactor()
      - printChainingTable(), printOpenTable()
      - homeIndex(), stepFor(), clusterLength()
      - insert(..., probeTrace) for Mock G
*/
template<class Key, class Value>
class HashTable {
    struct slot {
        Key key;
        Value value;
        slotState state;
        slot() : state(EMPTY) {}
    };

    typedef ll (*HashFunction)(const Key&);

    Method method;
    HashFunction hash_function;
    vector<list<pair<Key, Value>>> chainTable;
    vector<slot> table_probe;

    int count_item;
    long long totalCollisions;
    int insertsSinceGrow;
    int deletesSinceShrink;
    int insertsNeededToGrow;
    int deletesNeededToShrink;

    int get_step(const Key& key) const {
        ll value = hash_function(key);
        return 1 + (value >> 16) % (capacity() - 1);
    }

    int get_prob_index(const Key& key, int attempt) const {
        int m = capacity();
        ll initial_pos = hash_function(key) % m;
        ll step = get_step(key);

        if (method == DOUBLE_HASHING)
            return (initial_pos + 1ULL * attempt * step) % m;

        const int C1 = 17, C2 = 31;
        return (initial_pos + C1 * 1ULL * attempt * step
                + C2 * 1ULL * attempt * attempt) % m;
    }

    bool reinsert(const Key& key, const Value& value) {
        if (method == CHAINING) {
            int index = hash_function(key) % capacity();
            chainTable[index].push_back({key, value});
            return true;
        }

        for (int i = 0; i < capacity(); i++) {
            slot& currentSlot = table_probe[get_prob_index(key, i)];
            if (currentSlot.state != OCCUPIED) {
                currentSlot.key = key;
                currentSlot.value = value;
                currentSlot.state = OCCUPIED;
                return true;
            }
        }
        return false;
    }

    void table_resize(int newSize) {
        vector<pair<Key, Value>> oldItems;

        if (method == CHAINING) {
            for (auto& bucket : chainTable)
                for (auto& item : bucket)
                    oldItems.push_back(item);

            chainTable.clear();
            chainTable.resize(newSize);
        }
        else {
            for (auto& currentSlot : table_probe)
                if (currentSlot.state == OCCUPIED)
                    oldItems.push_back({currentSlot.key, currentSlot.value});

            while (true) {
                table_probe.clear();
                table_probe.resize(newSize);

                bool success = true;
                for (auto& item : oldItems) {
                    if (!reinsert(item.first, item.second)) {
                        success = false;
                        break;
                    }
                }

                if (success) return;
                newSize = nextPrime(newSize * 2);
            }
        }

        for (auto& item : oldItems)
            reinsert(item.first, item.second);
    }

    int capacity() const {
        if (method == CHAINING) return (int)chainTable.size();
        return (int)table_probe.size();
    }

    double loadFactor() const {
        return (double)count_item / capacity();
    }

    void table_shrink() {
        int newSize = previousPrime(capacity() / 2);
        if (newSize < size_init) newSize = size_init;
        table_resize(newSize);
        deletesNeededToShrink = max(1, count_item / 2);
        deletesSinceShrink = 0;
    }

    void shrink_check() {
        if (capacity() > size_init &&
            loadFactor() < minimum_load &&
            deletesSinceShrink >= deletesNeededToShrink)
            table_shrink();
    }

    void table_grow() {
        table_resize(nextPrime(capacity() * 2));
        insertsNeededToGrow = max(1, count_item / 2);
        insertsSinceGrow = 0;
    }

    void grow_check() {
        if (loadFactor() > highest_load &&
            insertsSinceGrow >= insertsNeededToGrow)
            table_grow();
    }

public:
    HashTable(Method chosenMethod, HashFunction chosenHash) {
        method = chosenMethod;
        hash_function = chosenHash;
        count_item = 0;
        totalCollisions = 0;
        insertsSinceGrow = deletesSinceShrink = 0;
        insertsNeededToGrow = deletesNeededToShrink = 0;

        if (method == CHAINING) chainTable.resize(size_init);
        else table_probe.resize(size_init);
    }

    long long collisions() const { return totalCollisions; }
    int getCapacity() const { return capacity(); }
    int getCount() const { return count_item; }
    double getLoadFactor() const { return loadFactor(); }
    int homeIndex(const Key& key) const {
        return hash_function(key) % capacity();
    }
    int stepFor(const Key& key) const { return get_step(key); }

    bool search(const Key& key, Value& foundValue, int& hits) const {
        hits = 0;

        if (method == CHAINING) {
            int index = hash_function(key) % capacity();
            for (const auto& item : chainTable[index]) {
                hits++;
                if (item.first == key) {
                    foundValue = item.second;
                    return true;
                }
            }
            return false;
        }

        for (int i = 0; i < capacity(); i++) {
            const slot& currentSlot = table_probe[get_prob_index(key, i)];
            hits++;
            if (currentSlot.state == EMPTY) return false;
            if (currentSlot.state == OCCUPIED && currentSlot.key == key) {
                foundValue = currentSlot.value;
                return true;
            }
        }
        return false;
    }

    bool insert(const Key& key, const Value& value,
                vector<int>* probeTrace = nullptr,
                vector<Key>* occupants = nullptr) {
        int hits;
        Value oldValue;
        if (search(key, oldValue, hits)) return false;

        if (method == CHAINING) {
            int index = hash_function(key) % capacity();
            if (!chainTable[index].empty()) totalCollisions++;
            chainTable[index].push_back({key, value});
        }
        else {
            bool inserted = false;
            while (!inserted) {
                for (int i = 0; i < capacity(); i++) {
                    int position = get_prob_index(key, i);
                    slot& currentSlot = table_probe[position];

                    if (probeTrace) probeTrace->push_back(position);
                    if (occupants) {
                        if (currentSlot.state == OCCUPIED)
                            occupants->push_back(currentSlot.key);
                        else
                            occupants->push_back(Key{});
                    }

                    if (currentSlot.state == OCCUPIED) {
                        totalCollisions++;
                    }
                    else {
                        currentSlot.key = key;
                        currentSlot.value = value;
                        currentSlot.state = OCCUPIED;
                        inserted = true;
                        break;
                    }
                }

                if (!inserted) table_grow();
            }
        }

        count_item++;
        insertsSinceGrow++;
        grow_check();
        return true;
    }

    bool erase(const Key& key) {
        if (method == CHAINING) {
            int index = hash_function(key) % capacity();
            auto& bucket = chainTable[index];

            for (auto it = bucket.begin(); it != bucket.end(); it++) {
                if (it->first == key) {
                    bucket.erase(it);
                    count_item--;
                    deletesSinceShrink++;
                    shrink_check();
                    return true;
                }
            }
            return false;
        }

        for (int i = 0; i < capacity(); i++) {
            slot& currentSlot = table_probe[get_prob_index(key, i)];
            if (currentSlot.state == EMPTY) return false;

            if (currentSlot.state == OCCUPIED && currentSlot.key == key) {
                currentSlot.state = DELETED;
                count_item--;
                deletesSinceShrink++;
                shrink_check();
                return true;
            }
        }
        return false;
    }

    int clusterLength(const Key& key) const {
        if (method == CHAINING) return -1;

        int home = homeIndex(key);
        int length = 0;

        while (length < capacity()) {
            int position = (home + length) % capacity();
            if (table_probe[position].state == EMPTY) break;
            length++;
        }
        return length;
    }

    void printChainingTable() const {
        if (method != CHAINING) return;

        for (int i = 0; i < capacity(); i++) {
            if (chainTable[i].empty()) continue;

            cout << "Bucket " << i << " : ";
            bool first = true;
            for (const auto& item : chainTable[i]) {
                if (!first) cout << " -> ";
                cout << item.first << "(" << item.second << ")";
                first = false;
            }
            cout << '\n';
        }
    }

    void printOpenTable() const {
        if (method == CHAINING) return;

        for (int i = 0; i < capacity(); i++) {
            cout << "Slot " << i << ": ";
            if (table_probe[i].state == OCCUPIED)
                cout << table_probe[i].key << "(" << table_probe[i].value << ")";
            else if (table_probe[i].state == DELETED)
                cout << "DELETED";
            else
                cout << "EMPTY";
            cout << '\n';
        }
    }
};

string randomWord(int length) {
    const string letters = "abcdefghijklmnopqrstuvwxyz";
    string word;
    for (int i = 0; i < length; i++)
        word += letters[rand() % 26];
    return word;
}

vector<string> unique_word_generator(int amount, int length) {
    vector<string> words;
    unordered_set<string> usedWords;

    while ((int)words.size() < amount) {
        string word = randomWord(length);
        if (usedWords.insert(word).second)
            words.push_back(word);
    }
    return words;
}

// ============================================================================
// MOCK A - BUCKET-CAPACITY EVICTION
// Specification:
// 1. Use position-weighted Hash1, N=13, and the Section-B weight function.
// 2. Every bucket holds at most BUCKET_CAP=2 live keys.
// 3. If full, a strictly heavier new key evicts the minimum-weight key.
//    If minimum weights tie, evict the one inserted earlier.
// 4. Otherwise discard the new key.
// 5. Keep each chain sorted by descending weight; weight ties keep earlier first.
// 6. Insert: cat,dog,bat,rat,sun,fun,run,top,pot,opt,art,tar.
// ============================================================================
struct WeightedNode {
    string key;
    int value;
    double weight;
    int order;
};

class BoundedWeightedTable {
    static const int N = 13;
    static const int BUCKET_CAP = 2;
    vector<list<WeightedNode>> table;
    vector<string> discarded;
    int insertionOrder = 0;

    void sortBucket(list<WeightedNode>& bucket) {
        bucket.sort([](const WeightedNode& a, const WeightedNode& b) {
            if (a.weight != b.weight) return a.weight > b.weight;
            return a.order < b.order;
        });
    }

public:
    BoundedWeightedTable() : table(N) {}

    void insert(const string& key, int value) {
        int index = positionHash(key);
        auto& bucket = table[index];

        for (const auto& node : bucket)
            if (node.key == key) return;

        WeightedNode newNode{key, value, weight_cal(key), insertionOrder++};

        if ((int)bucket.size() < BUCKET_CAP) {
            bucket.push_back(newNode);
            sortBucket(bucket);
            return;
        }

        auto victim = bucket.begin();
        for (auto it = bucket.begin(); it != bucket.end(); it++) {
            if (it->weight < victim->weight ||
                (it->weight == victim->weight && it->order < victim->order))
                victim = it;
        }

        if (newNode.weight > victim->weight) {
            discarded.push_back(victim->key);
            bucket.erase(victim);
            bucket.push_back(newNode);
            sortBucket(bucket);
        }
        else {
            discarded.push_back(key);
        }
    }

    void print() const {
        cout << fixed << setprecision(2);
        for (int i = 0; i < N; i++) {
            if (table[i].empty()) continue;
            cout << "Bucket " << i << " : ";
            bool first = true;
            for (const auto& node : table[i]) {
                if (!first) cout << " -> ";
                cout << node.key << " (" << node.weight << ")";
                first = false;
            }
            cout << '\n';
        }

        cout << "Discarded keys: ";
        if (discarded.empty()) cout << "None";
        for (int i = 0; i < (int)discarded.size(); i++) {
            if (i) cout << ", ";
            cout << discarded[i];
        }
        cout << '\n';
    }
};

void solveMockA() {
    vector<string> words = {
        "cat", "dog", "bat", "rat", "sun", "fun",
        "run", "top", "pot", "opt", "art", "tar"
    };

    BoundedWeightedTable table;
    for (int i = 0; i < (int)words.size(); i++)
        table.insert(words[i], i + 1);
    table.print();
}

// ============================================================================
// MOCK B - SET OPERATIONS, EXTENDED
// Specification:
// 1. Read |A|, A, |B|, B.
// 2. Use hash-table insert/search; no nested A-by-B scan.
// 3. Print symmetric difference (elements in exactly one set).
// 4. Print whether A is a subset of B and B is a subset of A.
// Expected complexity: O(|A|+|B|).
// ============================================================================
void solveMockB() {
    int n, m;
    cin >> n;
    vector<int> A(n);
    for (int& x : A) cin >> x;
    cin >> m;
    vector<int> B(m);
    for (int& x : B) cin >> x;

    HashTable<int, int> tableA(CHAINING, integerHash);
    HashTable<int, int> tableB(CHAINING, integerHash);

    for (int x : A) tableA.insert(x, x);
    for (int x : B) tableB.insert(x, x);

    vector<int> symmetricDifference;
    bool aSubsetB = true, bSubsetA = true;
    int value, hits;

    for (int x : A) {
        if (!tableB.search(x, value, hits)) {
            symmetricDifference.push_back(x);
            aSubsetB = false;
        }
    }

    for (int x : B) {
        if (!tableA.search(x, value, hits)) {
            symmetricDifference.push_back(x);
            bSubsetA = false;
        }
    }

    cout << "Symmetric Difference:";
    for (int x : symmetricDifference) cout << ' ' << x;
    cout << "\nIs A subset of B? " << (aSubsetB ? "Yes" : "No");
    cout << "\nIs B subset of A? " << (bSubsetA ? "Yes" : "No") << '\n';
}

// ============================================================================
// MOCK C - CLUSTERING REPORT FOR OPEN ADDRESSING
// Specification:
// 1. home = hash_function(key) mod N.
// 2. From home, walk linearly (home+1, wrapping) through OCCUPIED/DELETED
//    slots until the first EMPTY slot. That count is clusterLength.
// 3. Read n query keys and print key, home, and cluster length.
// 4. Print the first queried key having the maximum cluster length.
// 5. Chaining would report that cluster length is not applicable.
// This driver builds CUSTOM_PROBING with 10,000 unique random length-10 words.
// ============================================================================
void solveMockC() {
    srand((unsigned)time(nullptr));
    vector<string> words = unique_word_generator(10000, 10);
    HashTable<string, int> table(CUSTOM_PROBING, hash1);

    for (int i = 0; i < (int)words.size(); i++)
        table.insert(words[i], i + 1);

    int n;
    cin >> n;

    string bestKey;
    int maximumLength = -1;

    while (n--) {
        string key;
        cin >> key;
        int length = table.clusterLength(key);

        cout << key << " -> home=" << table.homeIndex(key)
             << ", clusterLength=" << length << '\n';

        if (length > maximumLength) {
            maximumLength = length;
            bestKey = key;
        }
    }

    cout << "Maximum cluster length: " << maximumLength
         << " (key: " << bestKey << ")\n";
}

// ============================================================================
// MOCK D - FREQUENCY-COUNTING CHAINS
// Specification:
// 1. Use position-weighted Hash1 with fixed N=13.
// 2. A duplicate increments the existing node's frequency; no second node.
// 3. Sort every bucket by descending frequency, then alphabetical key.
// 4. Print non-empty buckets and global top 3 with the same ordering rule.
// 5. Insert: bat,fun,cat,sun,bat,dog,fun,rat,cat,bat,fun,rat.
// ============================================================================
struct FrequencyNode {
    string key;
    int frequency;
};

class FrequencyTable {
    static const int N = 13;
    vector<list<FrequencyNode>> table;

    static bool comesBefore(const FrequencyNode& a, const FrequencyNode& b) {
        if (a.frequency != b.frequency) return a.frequency > b.frequency;
        return a.key < b.key;
    }

public:
    FrequencyTable() : table(N) {}

    void insert(const string& key) {
        int index = positionHash(key);
        auto& bucket = table[index];

        for (auto& node : bucket) {
            if (node.key == key) {
                node.frequency++;
                bucket.sort(comesBefore);
                return;
            }
        }

        bucket.push_back({key, 1});
        bucket.sort(comesBefore);
    }

    void print() const {
        vector<FrequencyNode> allWords;

        for (int i = 0; i < N; i++) {
            if (table[i].empty()) continue;
            cout << "Bucket " << i << " : ";

            bool first = true;
            for (const auto& node : table[i]) {
                if (!first) cout << " -> ";
                cout << node.key << " (" << node.frequency << ")";
                first = false;
                allWords.push_back(node);
            }
            cout << '\n';
        }

        sort(allWords.begin(), allWords.end(), comesBefore);
        cout << "Top 3 most frequent: ";
        for (int i = 0; i < min(3, (int)allWords.size()); i++) {
            if (i) cout << ", ";
            cout << allWords[i].key << " (" << allWords[i].frequency << ")";
        }
        cout << '\n';
    }
};

void solveMockD() {
    vector<string> words = {
        "bat", "fun", "cat", "sun", "bat", "dog",
        "fun", "rat", "cat", "bat", "fun", "rat"
    };

    FrequencyTable table;
    for (const string& word : words) table.insert(word);
    table.print();
}

// ============================================================================
// MOCK E - CHAINING TRACE WITH BASE HASH1
// Specification:
// 1. Fixed starting N=13, base Hash1, CHAINING.
// 2. Append new keys to each chain in insertion order.
// 3. Discard duplicates.
// 4. For every word, print Hash1 mod 13 and inserted/discarded status.
// 5. Print count, capacity, load factor, collisions, and non-empty buckets.
// Input: kite,lamp,frog,zeal,gnome,wolf,frog,lamp.
// ============================================================================
void solveMockE() {
    vector<string> words = {
        "kite", "lamp", "frog", "zeal",
        "gnome", "wolf", "frog", "lamp"
    };

    HashTable<string, int> table(CHAINING, hash1);

    for (int i = 0; i < (int)words.size(); i++) {
        bool inserted = table.insert(words[i], i + 1);
        cout << words[i] << " -> Hash1 mod 13 = " << hash1(words[i]) % 13;
        if (inserted)
            cout << " -> inserted\n";
        else
            cout << " -> discarded (duplicate)\n";
    }

    cout << fixed << setprecision(4);
    cout << "\ncapacity = " << table.getCapacity()
         << ", count = " << table.getCount()
         << ", load factor = " << table.getLoadFactor()
         << ", collisions = " << table.collisions() << "\n\n";
    table.printChainingTable();
}

// ============================================================================
// MOCK F - GROW/SHRINK RESIZE TRACE
// Specification:
// 1. CHAINING + base Hash1. Initial capacity=13.
// 2. Grow if load>0.50 and cooldown allows; new size=nextPrime(2*capacity).
// 3. Shrink if load<0.25 and cooldown allows; new size is previous prime at
//    capacity/2, never smaller than 13.
// 4. After each insert/delete print count, capacity and load, flagging resize.
// Insert: kite,lamp,frog,zeal,gnome,wolf,idea,plum,dust.
// Delete: kite,lamp,frog,zeal,gnome,wolf.
// Note: the PDF sample has a small typo: 5/13 is 0.3846, not 0.3077.
// ============================================================================
void solveMockF() {
    vector<string> insertWords = {
        "kite", "lamp", "frog", "zeal", "gnome",
        "wolf", "idea", "plum", "dust"
    };
    vector<string> deleteWords = {
        "kite", "lamp", "frog", "zeal", "gnome", "wolf"
    };

    HashTable<string, int> table(CHAINING, hash1);
    cout << fixed << setprecision(4);

    for (int i = 0; i < (int)insertWords.size(); i++) {
        int oldCapacity = table.getCapacity();
        table.insert(insertWords[i], i + 1);
        int newCapacity = table.getCapacity();

        cout << "insert " << insertWords[i]
             << " -> count=" << table.getCount()
             << " capacity=" << newCapacity
             << " load=" << table.getLoadFactor();

        if (newCapacity > oldCapacity)
            cout << " *** GREW " << oldCapacity << " -> " << newCapacity << " ***";
        cout << '\n';
    }

    cout << '\n';
    for (const string& word : deleteWords) {
        int oldCapacity = table.getCapacity();
        table.erase(word);
        int newCapacity = table.getCapacity();

        cout << "delete " << word
             << " -> count=" << table.getCount()
             << " capacity=" << newCapacity
             << " load=" << table.getLoadFactor();

        if (newCapacity < oldCapacity)
            cout << " *** SHRANK " << oldCapacity << " -> " << newCapacity << " ***";
        cout << '\n';
    }
}

// ============================================================================
// MOCK G - CUSTOM PROBING TRACE
// Specification:
// customHash(k,i)=(home+17*i*step+31*i*i)%N
// home=Hash1(k)%N, step=1+((Hash1(k)>>16)%(N-1)), fixed N=13.
// 1. Insert kite,lamp,frog,zeal,gnome,wolf in order.
// 2. Print all slots 0..12.
// 3. For a word with a home-slot collision, print every attempted slot.
// 4. Print total insertion collisions.
// ============================================================================
void solveMockG() {
    vector<string> words = {"kite", "lamp", "frog", "zeal", "gnome", "wolf"};
    HashTable<string, int> table(CUSTOM_PROBING, hash1);

    struct Trace {
        string key;
        int home;
        int step;
        vector<int> positions;
        vector<string> occupants;
    };
    vector<Trace> collisionTraces;

    for (int i = 0; i < (int)words.size(); i++) {
        vector<int> positions;
        vector<string> occupants;
        int home = table.homeIndex(words[i]);
        int step = table.stepFor(words[i]);

        table.insert(words[i], i + 1, &positions, &occupants);

        if (positions.size() > 1)
            collisionTraces.push_back({words[i], home, step, positions, occupants});
    }

    table.printOpenTable();

    for (const auto& trace : collisionTraces) {
        cout << '\n' << trace.key
             << ": home = Hash1(" << trace.key << ") mod 13 = " << trace.home
             << ", step = " << trace.step << '\n';

        for (int i = 0; i < (int)trace.positions.size(); i++) {
            cout << "attempt " << i << " -> slot " << trace.positions[i];
            if (!trace.occupants[i].empty())
                cout << " (OCCUPIED by " << trace.occupants[i] << ", collision)";
            else
                cout << " (EMPTY -> " << trace.key << " inserted here)";
            cout << '\n';
        }
    }

    cout << "\nTotal collisions this run: " << table.collisions() << '\n';
}

// Choose one mock. B and C require the extra input described in their comments.
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    char choice;
    cout << "Choose mock question (A-G): ";
    cin >> choice;
    choice = toupper((unsigned char)choice);

    switch (choice) {
        case 'A': solveMockA(); break;
        case 'B': solveMockB(); break;
        case 'C': solveMockC(); break;
        case 'D': solveMockD(); break;
        case 'E': solveMockE(); break;
        case 'F': solveMockF(); break;
        case 'G': solveMockG(); break;
        default: cout << "Invalid choice. Enter A, B, C, D, E, F, or G.\n";
    }

    return 0;
}
