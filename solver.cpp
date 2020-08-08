#include"convert.h"
#include<chrono>

#define pvec(v) for(auto elem: v) cout << elem << " ";cout << endl;

using namespace std;
using namespace std::chrono;

vector<int> *tally;
string puzzle = "puzzles/grid5.txt";
int perfCounter = 0;

Node* chooseColumn(Node* root) {
    Node *lowest = nullptr, *temp = root->right;
    int mn = 1000000000;
    while (temp!=root) {
        if (temp->ones < mn) {
            mn = temp->ones;
            lowest = temp;
        }
        temp = temp->right;
    }
    return lowest;
}

void cover(Node *col) {
    col->right->left = col->left;
    col->left->right = col->right;

    Node *currRow = col->down, *temp;
    while (currRow!=col) {
        temp = currRow->right;
        while (temp!=currRow) {
            temp->colHeader->ones-=1;
            temp->up->down = temp->down;
            temp->down->up = temp->up;
            temp = temp->right;
        }
        currRow = currRow->down;
    }
}

void uncover(Node *col) {
    Node* currRow = col->up, *temp;

    while (currRow!=col) {
        temp = currRow->left;
        while (temp!=currRow) {
            temp->colHeader->ones+=1;
            temp->up->down = temp;
            temp->down->up = temp;
            temp = temp->left;
        }
        currRow = currRow->up;
    }
    col->left->right = col;
    col->right->left = col;
}

void printSolution(vector<int> &ps) {
    auto sudoku = getSudoku(puzzle);
    int **grid = sudoku.first, n = sudoku.second;
    // int grid[4][4], n = 4;

    for (auto elem: ps) {
        grid[tally[elem][0]][tally[elem][1]] = tally[elem][2];
    }

    fstream fout;
    fout.open("answers.txt", ios::out);
    for (int i = 0; i<n; ++i) {
        for (int j = 0; j<n; ++j)
            fout << grid[i][j] << " ";
        fout << "\n";
    }
    fout.close();
}

bool search(Node *root, vector<int> &partialSolution) {
    // No column left to cover
    perfCounter+=1;
    if ((root->left == root) && (root->right == root)) {
        printSolution(partialSolution);
        return true;
    }
    Node *col = chooseColumn(root), *curr = col->down, *currRow;
    cover(col);
    while (curr!=col) {
        partialSolution.push_back(curr->row);
        currRow = curr->right;
        while (currRow!=curr) {
            cover(currRow->colHeader);
            currRow = currRow->right;
        }

        if (search(root, partialSolution))
            return true;

        currRow = curr->left;
        while (currRow!=curr) {
            uncover(currRow->colHeader);
            currRow = currRow->left;
        }
        partialSolution.pop_back();

        curr = curr->down;
    }
    uncover(col);

    return false;
}

void printColWise(Node* root) {
    Node *temp = root->right;
    int cnt = 0;
    while (temp!=root) {
        Node *goDown = temp->down;
        while (goDown!=temp) {
            ++cnt;
            goDown = goDown->down;
        }
        temp = temp->right;
    }
}



int main() {
    auto start = high_resolution_clock::now();
    auto res = get_dlx(puzzle);
    Node *root = res.first;
    tally = res.second;
    vector<int> ps;

    if (!search(root, ps))
        cout << "No Solution Possible!!!";
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time: " << duration.count()/1000.0 << " ms" << endl;

    return 0;
}