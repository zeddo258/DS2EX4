#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <iomanip>


class Reader {
    struct Data {
        char CallerID[10]; 
        char CalleeID[10]; 
        float weight; 
        void Print() {
            std::cout<< CallerID << " " << CalleeID << " " << weight << "\n"; 
        }
    } ; 
    std::vector <Data> dataList; 
    int fileID; 
    
    public: 

    Reader() {
        fileID = 0; 
        dataList.clear(); 
    }

    void Print(int i ) {
        dataList[i].Print(); 
    } 

    char *Get_CallerID(int i) {
        return this -> dataList[i].CallerID; 
    }

    char *Get_CalleeID(int i) {
        return this -> dataList[i].CalleeID; 
    }

    float Get_Weight(int i) {
        return this -> dataList[i].weight; 
    }

    int Get_fileID() {return this -> fileID; }

    int GetSize() {return this -> dataList.size(); }
    bool GetData(); 

    
} ; 


bool Reader::GetData() {
    std::string fileName; 
    std::ifstream inFile; 
    int id = -1; 
    
    std::cout << "\nInput a file number ([0] Quit):"; 
    std::cin >> id; 
    

    if (id == 0 ) return false; 

    if( id != 0 ) {
        this -> fileID = id; 
        fileName = "pairs" + std::to_string(id) + ".bin"; 
        inFile.open(fileName, std::ios::binary );

        if (!inFile.is_open()) {
            std::cout << "\n" << fileName << " does not exist."; 
            return false; 
        } 

        else {
            Data data; 
            while (inFile.read( (char*)&data, sizeof(data) ) ) { 
                dataList.push_back(data);
            }
            return true; 
        }

    }
    return false; 
}

class Vertex  {
    char caller[10]; 
    struct Callee {
        char callee[10]; 
        float weight; 
        // Calle constructor 
        Callee() {
            for (int i = 0; i < 10; i++ ) 
                callee[i] == '\0'; 
            weight = 0.0; 
        }
    } ; 
    std::vector <Callee> callee; 

    public:
    // Vertex constructor 
    Vertex() {
        for (int i = 0; i < 10; i++ ) 
            caller[i] == '\0'; 
        callee.clear(); 
    }

    // Get caller vertex 
    char *Get_CallerID() {return this -> caller; }
    int Get_CalleeSize() {return this -> callee.size();}
    // Vertex setter 

    char *Get_CalleeID(int i) {return this -> callee[i].callee;} 
    float Get_CalleeWeight(int i) {return this -> callee[i].weight;} 

    void Set_VertexCaller(char caller[10]);
    void Set_VertexCallee(char callee[10], float w); 

} ; 

void Vertex::Set_VertexCaller(char caller[10]) {
    strcpy(this->caller, caller); 
}

void Vertex::Set_VertexCallee(char callee[10], float w) {
    Callee c;
    strcpy(c.callee,callee); 
    c.weight = w; 
    this -> callee.push_back(c);
}

class Graph {
    std::vector <Vertex*> vertices;  
    public: 
        Graph() {vertices.clear(); }
        void formGraph(Reader rd);
        void WriteAdj(Reader rd); 
} ; 

void Graph::formGraph(Reader rd) {
    Vertex *v = new Vertex; 
    v->Set_VertexCaller(rd.Get_CallerID(0));
    v->Set_VertexCallee(rd.Get_CalleeID(0), rd.Get_Weight(0) ); 
    for (int i = 1; i < rd.GetSize(); i++ ) {

        if ( strcmp(v->Get_CallerID(), rd.Get_CallerID(i)) == 0 ) {
            v->Set_VertexCallee(rd.Get_CalleeID(i), rd.Get_Weight(i) ); 
        }

        else {
            vertices.push_back(v); 
            v = new Vertex; 
            v->Set_VertexCaller(rd.Get_CallerID(i));
            v->Set_VertexCallee(rd.Get_CalleeID(i), rd.Get_Weight(i) ); 
        }

        if (rd.GetSize() - i == 1 ) vertices.push_back(v); 

    }
}

void Graph::WriteAdj(Reader rd ) {
    int fileID = rd.Get_fileID(); 
    
    std::string fileName = "pairs" + std::to_string(fileID) + ".adj"; 
    std::ofstream outFile; 
    outFile.open(fileName); 
    outFile << "<<< There are " + std::to_string(vertices.size()) + " IDs in total. >>>";
    for(int i = 0; i < this -> vertices.size(); i++ ) {
        outFile << "\n[" + std::to_string(i+1) + "]"; 
        outFile << vertices[i]->Get_CallerID() << ": \n";
        for ( int j = 0; j < this -> vertices[i]->Get_CalleeSize(); j++ ) {
            // ( j+1)
            outFile << "\t(" << std::to_string(j+1) << ") "; 
            // id, 
            outFile << vertices[i]->Get_CalleeID(j) << ", "; 
            outFile << vertices[i]->Get_CalleeWeight(j); 
        }
        
    }
    outFile << "\n<<< There are " + std::to_string(rd.GetSize()) + " nodes in total. >>>";
}




int main() {
    Reader rd; 
    Graph *gr = new Graph(); 
    rd.GetData(); 
    gr->formGraph(rd); 
    gr->WriteAdj(rd); 
    return 0;
}