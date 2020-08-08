#include<fstream>
#include<iostream>
#include<bits/stdc++.h>

using namespace std;

struct Node{
    int ones, col, row;
    Node *right, *left, *up, *down, *colHeader;

    Node(int r = -1, int c = -1){
        right = left = up = down = nullptr;
        ones = 0;
        row = r;
        col = c;
    }
};

pair<int**, int> getSudoku(string &puzzle){
    int **grid;
    fstream fin;
    fin.open(puzzle, ios::in);

    int n;
    fin >> n;
    grid = new int*[n];
    for(int i = 0; i<n; ++i){
        grid[i] = new int[n]{0};
        for(int j = 0; j<n; ++j)
            fin >> grid[i][j];
    }

    fin.close();
    return {grid, n};
}

void connectLR(Node *a, Node *b){
    a->right = b;
    b->left = a;
}

void connectUD(Node *a, Node *b){
    a->down = b;
    b->up = a;
}

void coverup(Node *col){
    col->right->left = col->left;
    col->left->right = col->right;

    Node *currRow = col->down, *temp;
    while(currRow!=col){
        temp = currRow->right;
        while(temp!=currRow){
            temp->colHeader->ones-=1;
            temp->up->down = temp->down;
            temp->down->up = temp->up;
            temp = temp->right;
        }
        // cout << ">" << col->col << " " << currRow->row << endl;
        currRow = currRow->down;
    }
}


pair<Node*, vector<int>*> get_dlx(string &puzzle){
    auto sudoku = getSudoku(puzzle);
    int **grid = sudoku.first, n = sudoku.second;
    vector<Node*> columnHeader(4*n*n);

    vector<vector<Node*>> columnWise(4*n*n + 1);

    for(int i = 0; i<4*n*n; ++i){
        columnHeader[i] = new Node(0, i);
        columnWise[i].push_back(columnHeader[i]);
    }

    for(int i = 0; i<4*n*n - 1; ++i)
        connectLR(columnHeader[i], columnHeader[i+1]);

    
    int cellOffset = 0;
	int rowOffset = n*n;
	int colOffset = 2*n*n;
	int regOffset = 3*n*n;
    bool toBeRemoved[n*n*n + n] = {0};
    for(int i = 0; i<n; ++i){
        for(int j = 0; j<n; ++j){
            if(grid[i][j]!=0){
                int c = grid[i][j];
                toBeRemoved[i*(n*n) + j*n + c] = 1;
            }
        }
    }

    int counter = 0, sqn = (int)sqrt(n*1.0);
    vector<Node*> toRemove;
    vector<int> *tally = new vector<int>[n*n*n + 1];

    for(int row = 0; row<n; ++row){
        for(int col = 0; col<n; ++col){
            for(int num = 1; num < n + 1; ++num, ++counter){
                Node *fir, *sec, *thi, *fou;
                fir = new Node(counter, cellOffset + row*n + col); 
                fir->colHeader = columnHeader[cellOffset + row*n + col];
                columnWise[cellOffset + row*n + col].push_back(fir);

                sec = new Node(counter, rowOffset + row*n + num - 1);
                sec->colHeader = columnHeader[rowOffset + row*n + num - 1]; 
                columnWise[rowOffset + row*n + num - 1].push_back(sec);

                thi = new Node(counter, colOffset + col*n + num - 1); 
                thi->colHeader = columnHeader[colOffset + col*n + num - 1];
                columnWise[colOffset + col*n + num - 1].push_back(thi);

                fou = new Node(counter, regOffset + ((row/sqn)*sqn + col/sqn)*n + num - 1); 
                fou->colHeader = columnHeader[regOffset + ((row/sqn)*sqn + col/sqn)*n + num - 1];
                columnWise[regOffset + ((row/sqn)*sqn + col/sqn)*n + num - 1].push_back(fou);

                connectLR(fir, sec); connectLR(sec, thi); connectLR(thi, fou); connectLR(fou, fir);

                if(toBeRemoved[row*n*n + col*n + num]){
                    // cout << counter 
                    toRemove.push_back(fir);
                }
                vector<int> v = {row, col, num};
                tally[counter] = v;
            }
        }
    }

    for(int i = 0; i<4*n*n; ++i){
        int len = columnWise[i].size();
        for(int j = 0; j < len; ++j)
            connectUD(columnWise[i][j], columnWise[i][(j+1)%len]);
    }

    for(int i = 0; i<(int)toRemove.size(); ++i){
        Node *temp = toRemove[i]->right;
        coverup(toRemove[i]->colHeader);
        while(temp!=toRemove[i]){
            coverup(temp->colHeader);
            temp = temp->right;
        }
    }

    Node *root = new Node();
    root->right = columnHeader[0]; columnHeader[0]->left = root;
    root->left = columnHeader[4*n*n-1]; columnHeader[4*n*n-1]->right = root;
    return {root, tally};
}

