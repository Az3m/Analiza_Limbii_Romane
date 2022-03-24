#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <locale>
#include <vector>
#include <map>

using namespace std;

#define MAX_TREE_HT 100
//Functie sortare mapa
template<typename K, typename V>
std::multimap<V, K> invertMap(std::map<K, V> const& map)
{
    std::multimap<V, K> multimap;

    for (auto const& pair : map) {
        multimap.insert(std::make_pair(pair.second, pair.first));
    }

    return multimap;
}

map<char,int> CuratareText_ContorLitere(string nume)
{
    map<char, int> frecventa;
    fstream text;
    char litera;
    text.open(nume);
    if (text.is_open()) {
        while (!text.eof()) 
        {
                text >> litera;

                if (text.eof())
                    break;
                if ((int(litera) >= 65 && int(litera) <= 90)||(int(litera) >= 97 && int(litera)<=122))
                {
                    if ((int(litera) >= 97 && int(litera) <= 122)) {
                        
                            frecventa[litera-32]++;
                    }
                    else {
                        frecventa[litera]++;
                    }
                }
                else {
                    text.seekp((text.tellp() - static_cast<std::streampos>(1)));
                    text.put(' ');
                    text.seekp(text.tellp());
                }
                

            
        }
        text.close();
        return frecventa;
    }
    else {
        std::cout << "Faild to open" << '\n';
    }
}

map<char,double> FrecventaAparitie(string nume,map<char,int>mapa) {
    fstream text;
    text.open(nume);
    char litera;
    int litere=0,i=0;
    map<char,double> probabilitati;
    double frecventa,frecventaTotala=0,aparitii;
    while (!text.eof())
    {
        if (text.eof()) {
            break;
        }
        text >> litera;
        if (litera != ' ') {
            litere++;
        }
    }
    cout << "Textul contine " << litere << " litere."<<endl;
    map<char, int >::iterator itr;
    for (itr = mapa.begin(); itr != mapa.end(); itr++) {
        aparitii = itr->second;
        frecventa = aparitii / litere;
        litera = itr->first;
        frecventaTotala = frecventaTotala + frecventa;
        cout << "Litera " << itr->first << " are frecventa " << frecventa << endl;
        cout << "Frecventa totala este " << frecventaTotala <<endl ;
        probabilitati.insert({litera,frecventa});
    }
    return probabilitati;
}

void DateFinale(map<char,double> probabilitati, vector< pair<char, string> >coduriHuffman)
{
    double entropie = 0,lungimeMedie=0,eficienta,redundanta;
    int i;
    map<char, double >::iterator itr;
    cout << "<----------------------------------------------------->" << endl;
    
    for (itr = probabilitati.begin(); itr != probabilitati.end(); itr++) {
        entropie = entropie + (itr->second * log2(1 / itr->second));
    }
    cout << "Entropia este " << entropie << " [biti/caracter]" << endl;

    for (itr = probabilitati.begin(),i=0; itr != probabilitati.end(); itr++,i++) {
        lungimeMedie += itr->second * coduriHuffman[i].second.length();
    }
    cout << "<----------------------------------------------------->" << endl;
    cout << "Lungimea medie a combinatiilor de cod este " << lungimeMedie << " [biti/caracter]" << endl;

    eficienta = entropie / lungimeMedie;
    cout << "<----------------------------------------------------->" << endl;
    cout << "Eficienta este " << eficienta<< endl;

    redundanta = 1 - eficienta;
    cout << "<----------------------------------------------------->" << endl;
    cout << "Redundanta este " << redundanta << endl;

    cout << "<----------------------------------------------------->" << endl;
    cout << "Teorema lui Shannon" << endl;
    cout << entropie/log2(2) << "<=" << lungimeMedie-1 << "<=" << 1 + entropie / log2(2)<<endl;
    cout << "<----------------------------------------------------->" << endl;

}

//nod din arborele huffman
struct MinHeapNode {
    char data; //caracterul curent
    unsigned freq;//frecventa caracterului curent
    struct MinHeapNode* left, * right; //copilul stang si drept al nodului curent
};
//colectie de noduri huffman
struct MinHeap {
    unsigned size;//marimea curenta a minheal-ului
    unsigned capacity;//capacitatea minheap-ului
    struct MinHeapNode** array;//array de pointeri la minheapnode
};

//functie pentru alocat nod nou cu data si freq sale
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

//functie pentru a creea un minheap cu o capacitate data
struct MinHeap* createMinHeap(unsigned capacity){

    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));

    //marimea curenta este 0
    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

//functie pentru a interschimba 2 noduri
void swapMinHeapNode(struct MinHeapNode** a,
                     struct MinHeapNode** b)

{

    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx)

{

    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->
        freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->
        freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest],
            &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap)
{

    return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)

{

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]
        = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct MinHeap* minHeap,
    struct MinHeapNode* minHeapNode)

{

    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap* minHeap)

{

    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}



int isLeaf(struct MinHeapNode* root)

{

    return !(root->left) && !(root->right);
}

struct MinHeap* createAndBuildMinHeap(vector<char> data,vector <int> freq, int size)

{

    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}
struct MinHeapNode* buildHuffmanTree(vector<char> data,vector<int> freq, int size)

{
    struct MinHeapNode* left, * right, * top;

    // Step 1: Create a min heap of capacity
    // equal to size. Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {

        // Step 2: Extract the two minimum
        // freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3: Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}

vector< pair<char, string> >coduriHuffman;
vector< pair<char, string> > printCodes(struct MinHeapNode* root, int arr[], int top)
{
    string codHuffman;
    int i;

    // Assign 0 to left edge and recur
    if (root->left) {

        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
        
    }

    // Assign 1 to right edge and recur
    if (root->right) {

        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if (isLeaf(root)) {
        cout << root->data << ": ";
        for (i = 0; i < top; ++i) {
            //cout <<arr[i];
            codHuffman += to_string(arr[i]);

        }
        cout << codHuffman << endl;
        coduriHuffman.push_back(make_pair((char)root->data, codHuffman));

    }
    return coduriHuffman;
}

vector< pair<char, string> > HuffmanCodes(vector<char> data,vector <int> freq, int size)

{
    // Construct Huffman Tree
    struct MinHeapNode* root
        = buildHuffmanTree(data, freq, size);

    // Print Huffman codes using
    // the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;
    vector< pair<char, string> > coduriHuffman = printCodes(root, arr, top);
    return coduriHuffman;
}

void Codor(string nume, vector< pair<char, string> > coduriHuffman) {
    fstream Codor;
    char litera;
    string propozitie;
    Codor.open(nume);
    cout << "Mesajul este: " << endl<<endl;
    getline(Codor,propozitie);
    cout << propozitie <<endl;
    Codor.clear();
    Codor.seekg(0);
    if (Codor.is_open()) {
        while (!Codor.eof())
        {

            Codor >> litera;
            if (Codor.eof())
                break;
            for (int i = 0; i < coduriHuffman.size(); i++) {
                if (int(litera) == int(coduriHuffman[i].first) || int(litera) - 32 == int(coduriHuffman[i].first)) {
                    cout << coduriHuffman[i].second << '.';
                }
            }
        }
        cout << endl << endl;
        cout << "<----------------------------------------------------->" << endl;
    }
    else {
        std::cout << "Faild to open" << '\n';
    }
}

void Decodor(string nume, vector< pair<char, string> > coduriHuffman) {
    fstream Decodor;
    char litera;
    string propozitie;
    Decodor.open(nume);
    cout << "Mesajul este: " << endl << endl;
    getline(Decodor, propozitie);
    cout << propozitie << endl;
    propozitie.clear();
    Decodor.clear();
    Decodor.seekg(0);

    if (Decodor.is_open()) {
        while (!Decodor.eof())
        {
            Decodor >> litera;
            if (litera != '.') {
                propozitie.push_back(litera);
            }
            else {
                for (int i = 0; i < coduriHuffman.size(); i++) {
                    if (propozitie.compare(coduriHuffman[i].second) == 0) {
                        cout << coduriHuffman[i].first << '.';
                        propozitie.clear();
                    }
                }
            }
        }
        cout << endl << endl;
        cout << "<----------------------------------------------------->" << endl;
    }
    else {
        std::cout << "Faild to open" << '\n';
    }
}


int main() {

    //Numele fisierului care este citit
    string nume = "MoaraCuNoroc.txt";

    //numararea literelor si aparitiile lor
	map<char,int>litere = CuratareText_ContorLitere(nume);

    //Afisarea literelor si numarul fiecaruia de aparitii
    map<char, int >::iterator itr;
    for (itr = litere.begin(); itr != litere.end(); itr++) {
        cout << itr->first << " - " << itr->second << endl;
    }
    //Fac o mapa cu cheie Litera si frecventa sa
    map<char,double> probabilitati=FrecventaAparitie(nume,litere);

    //Sorteaza mapa cu litere in ordine crescatoare
    multimap<int, char> mapaSortata = invertMap(litere);

    //Introduc in 2 vectori litera si numarul ei de aparitii
    vector <char> arr;
    vector <int> freq;
    for (auto const& pair : mapaSortata) {
        arr.push_back(pair.second);
        freq.push_back(pair.first);
    }
    int size = arr.size();

    //Calcul coduri Huffman
    vector< pair<char, string> > coduriHuffman = HuffmanCodes(arr, freq, size);
    
    //Afisare entopie,lungime medie,eficienta,redundanta,teorema Shannon
    DateFinale(probabilitati,coduriHuffman);

    //Codificare mesaj din fisier text
    string numeCodor = "MesajPentruCodat.txt";
    Codor(numeCodor, coduriHuffman);

    //Decodificare mesaj din fisier text
    string numeDecodor = "MesajPentruDecodat.txt";
    Decodor(numeDecodor, coduriHuffman);
    


	return 0;
}