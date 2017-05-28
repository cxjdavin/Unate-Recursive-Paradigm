#include <assert.h>
#include <iostream>
#include <vector>

using namespace std;

#define POS       1   // Positive
#define NEG       -1  // Negative
#define DONTCARE  0   // Don't care

void const PrintF(const vector< vector<int> >& f) {
  for (vector<int> cube : f) {
    printf("[ ");
    for (int x : cube) {
      printf("%d ", x); 
    }
    printf("]\n");
  }
}

void const PrintVec(const vector<int>& v) {
  printf("< ");
  for (int x : v) {
    printf("%d ", x);
  }
  printf(">\n");
}

bool Is_Unate(vector< vector<int> > f) {
  vector<int> signs;

  // Init all variable signs to be DONTCARE
  for (int i = 0; i < f[0].size(); ++i) {
    signs.push_back(DONTCARE);
  }

  // All variables must have same sign in all cubes
  for (vector<int> cube : f) {
    for (int i = 0; i < cube.size(); ++i) {
      if (signs[i] != DONTCARE and signs[i] != cube[i]) {
        return false;
      } else {
        signs[i] = cube[i];
      }
    }
  }

  return true;
}

vector< vector<int> > Branch(vector< vector<int> >& f, int x, int sign) {
  vector< vector<int> > output;
  for (vector<int> cube : f) {
    if (cube[x] != DONTCARE and cube[x] != sign) {
      continue;
    }
    vector<int> new_cube(cube);
    new_cube[x] = DONTCARE;
    output.push_back(new_cube);
  }
  return output;
}

bool URP(vector< vector<int> > f, vector<int> branchable) {
  printf("\n");
  PrintF(f);
  printf("Branchable: "); PrintVec(branchable);
  printf("\n");

  if (Is_Unate(f)) {
printf("Is Unate\n");
    // If f is unate, check unate termination rules
    // If exists DONTCARE cube, return true
    // Else, return false
    bool has_dc_cube = false;
    for (vector<int> cube : f) {
      has_dc_cube = true;
      for (int x : cube) {
        if (x != DONTCARE) {
          has_dc_cube = false;
          break;
        }
      }
      if (has_dc_cube) {
        return true;
      }
    }
    return false;
  } else {
printf("Not Unate\n");
    //Recurse
    // Pick branching variable x
    if (branchable.size() == 0) assert(false);
    // For now just pick from back
    int x = branchable.back();
printf("Branching on %d\n", x);
    // Branch x=1
    vector< vector<int> > f1 = Branch(f, x, POS);
printf("Left branch (%d == POS):\n", x);
PrintF(f1); 
    // Branch x=0
    vector< vector<int> > f2 = Branch(f, x, NEG);
printf("Right branch (%d == NEG):\n", x);
PrintF(f2); 
    // Return true iff both branches return true
    vector<int> new_branchable(branchable);
    new_branchable.pop_back();
    return URP(f1, new_branchable) and URP(f2, new_branchable);
  }
}

/*
f = a' + bc + ab
represented as
[   a    b    c
  [NEG,  DC,  DC],
  [ DC, POS, POS],
  [POS, POS,  DC]
]
*/
bool RunF1() {
  vector< vector<int> > f;
  vector<int> cube1 = {NEG, DONTCARE, DONTCARE};
  vector<int> cube2 = {DONTCARE, POS, POS};
  vector<int> cube3 = {POS, POS, DONTCARE};
  f.push_back(cube1);
  f.push_back(cube2);
  f.push_back(cube3);

  vector<int> branchable;
  branchable.push_back(0);
  branchable.push_back(1);
  branchable.push_back(2);

  return URP(f, branchable);
}

/*
f = ab + ac + ab'c' + a'
represented as
[   a    b    c
  [POS, POS,  DC],
  [POS,  DC, POS],
  [POS, NEG, NEG],
  [NEG,  DC,  DC]
]
*/
bool RunF2() {
  vector< vector<int> > f;
  vector<int> cube1 = {POS, POS, DONTCARE};
  vector<int> cube2 = {POS, DONTCARE, POS};
  vector<int> cube3 = {POS, NEG, NEG};
  vector<int> cube4 = {NEG, DONTCARE, DONTCARE};
  f.push_back(cube1);
  f.push_back(cube2);
  f.push_back(cube3);
  f.push_back(cube4);

  vector<int> branchable;
  branchable.push_back(0);
  branchable.push_back(1);
  branchable.push_back(2);

  return URP(f, branchable);
}

int main() {
  printf("Is F1 a tautology? %s\n", RunF1() ? "Yes" : "No");
  printf("---------------------\n");
  printf("Is F2 a tautology? %s\n", RunF2() ? "Yes" : "No");
  return 0;
}
