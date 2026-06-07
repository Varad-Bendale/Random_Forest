#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include<algorithm>
#include <variant>
#include<set>
#include<cmath>
#include<omp.h>
using namespace std;



struct TreeNode {
  TreeNode* left;
    TreeNode* right;
    float value;
    float* leaf_status_left ;
    float*leaf_status_right ;  
      ~TreeNode() {
        delete left;
        delete right;
        delete leaf_status_left;
        delete leaf_status_right;
    }
    TreeNode() : left(nullptr), right(nullptr), 
                 leaf_status_left(nullptr), 
                 leaf_status_right(nullptr), 
                 value(0.0f) {}
}; 

unordered_map<float,int>used_variables ;
void leaf(TreeNode*tree  , unordered_map<float , pair<float , float>>leaf_info);

bool isnum (string datapart){
     if( datapart.size() == 0 ){
      return false ; 
     }
     bool dot = false ; 
     for (int i = 0 ; i < datapart.size() ; i++ ){
     char st = datapart[i] ; 
     if (i == 0 ){
     if ( isdigit(st) ==  0 && st != '-' && st != '.') {
        return false ; 
     }
     if (st == '.'){
        dot = true ; 
     }
    }
    else {
     if ( isdigit(st) ==  0 && st != '.' ) {
        return false ; 
     }
     if (st == '.' && dot == true){
        return false ; 
     }
     if (st == '.'){
        dot = true ; 
     }
     }
    }
     return true  ; 
}


vector<vector<float>> training_data() {
   vector<unordered_map<string,float>>hash   ; 
   string line  ;
   int i = 0 ;
   vector<vector<float>>data ; 
   vector<float> code  ; 
   ifstream file("train_data.csv");
    bool first = false ; 
    while (getline(file, line)) {
     stringstream ss(line);
     string datapart;
      vector<float> row ;
      bool skip = false;  
      int i = 0 ; 
     while (getline(ss, datapart, ',')) {
        if (first == false){
             hash.push_back(unordered_map<string,float>()); 
            code.push_back(0.0);
        }
        if (datapart.size() == 0 ){
            skip = true ; 
            break ; 
        }
        if (isnum(datapart) == true){
            row.push_back(stof(datapart)) ;
        }
        else {
            if (hash[i].count(datapart)  == 0 ){
                code[i] = code[i] + 1.0 ; 
                hash[i][datapart] = code[i]  ; 
                row.push_back(code[i]) ; 
            }
            else {
                row.push_back(hash[i][datapart])  ; 
            }

        }
        i++ ; 
        first = true ; 
    }
    if (skip == false ){
        data.push_back(row) ; 
    }
} 
return data ; 
}

vector<vector<float>> testing_data() {
   vector<unordered_map<string,float>>hash   ; 
   string line  ;
   int i = 0 ;
   vector<vector<float>>data ; 
   vector<float> code  ; 
   ifstream file("test_data.csv");
    bool first = false ; 
    while (getline(file, line)) {
     stringstream ss(line);
     string datapart;
      vector<float> row ;
      bool skip = false;  
      int i = 0 ; 
     while (getline(ss, datapart, ',')) {
        if (first == false){
             hash.push_back(unordered_map<string,float>()); 
            code.push_back(0.0);
        }
        if (datapart.size() == 0 ){
            skip = true ; 
            break ; 
        }
        if (isnum(datapart) == true){
            row.push_back(stof(datapart)) ;
        }
        else {
            if (hash[i].count(datapart)  == 0 ){
                code[i] = code[i] + 1.0 ; 
                hash[i][datapart] = code[i]  ; 
                row.push_back(code[i]) ; 
            }
            else {
                row.push_back(hash[i][datapart])  ; 
            }

        }
        i++ ; 
        first = true ; 
    }
    if (skip == false ){
        data.push_back(row) ; 
    }
} 
return data ; 
} 

float gini_impurity_subtree(vector<float>&counts ){
    int n = counts.size() ; 
    float sum = 0.0 ; 
    float gini = 0.0  ; 
    for(int i  = 0 ; i< n ; i++){
     sum = sum + counts[i] ; 
    }
    if (sum == 0.0){
        return 0.0 ; 
    }
    for(int i = 0 ; i < n ; i++){
     gini = gini + pow(counts[i]/sum , 2) ; 
    }
    gini = 1 - gini ; 
    return gini ;
}

#pragma omp parallel for
for (int i = 0; i < data.size(); i++) {
    vector<float> ans(5);
    vector<float>target_things ;  
    vector<float>smallest_gini_imp(7) ; 
    vector<pair<float  , float>> pr ; 
    vector<pair<float  , float>> pos ; 
    vector<float> final_ans ; 
    vector<float > hash_left(n,0) ;
    vector< float > hash_right(n,0) ; 
    
}

void  gini_impurity_tree(vector<float> &right_subtree , vector<float>& left_subtree){
    int n = right_subtree.size() ; 
    int m = left_subtree.size() ;
    float max_right  = 0.0 ; 
    float r = 0.0 ; 
     int a  = 0 ; 
    int b = 0 ; 
    for (int i = 0 ; i< n ; i++  ){
    r = r + right_subtree[i] ; 
    max_right = max(max_right ,right_subtree[i] ) ;
    if (max_right == right_subtree[i]){
           a = i  ; 
    }
    }
    float l = 0.0 ; 
    float max_left = 0.0 ;
    for (int j = 0 ; j< m ; j++ ){
        l = l + left_subtree[j] ; 
        max_left = max(max_left ,left_subtree[j] ) ;
        if (max_left == left_subtree[j]){
           b = j  ; 
        }
    }
    float tot = r+l ; 
    if (tot == 0.0) {             
        ans[0] = 0.0;
        ans[1] = 0.0;
        ans[2] = 0.0;
        ans[3] = 0.0;
        ans[4] = 0.0; 
        
        return ans;
    }
    float right = gini_impurity_subtree(right_subtree) ; 
    float left = gini_impurity_subtree(left_subtree) ; 
    float gini_final = (r/tot)*(right) + (l/tot)*(left ) ; 
    ans[0] = gini_final ;
    ans[1] = left ;
    ans[2] = right ;
    ans[3] = (float)a;
    ans[4] = (float)b; 

}

void perfect_variable(  unordered_map<float , pair<float , float>>leaf_info ,  vector<vector<pair<float , pair<float,float>>>> & data_new){
    int n = data_new.size() ; 
    vector<float>row  ; 
    float variable_pos   = 0.0 ; 
    float left_leaf = 0.0 ; 
    float right_leaf = 0.0 ; 
    float thresh_data_loc = 0.0 ; 
    vector<float>final_ans ; 
    float gini_left  = 0.0 ; 
    float gini_right = 0.0 ; 
     float gini ; 
     float gini_impurity  = 100 ; 
     float num  = 0.0 ; 
    pr.clear() ; 
    pos.clear() ; 
    for ( int i = 0 ; i< n-1 ; i++){
        if (data_new[i].size() <= 1 ){
            continue ; 
        }
        
        for (int j = 0  ; j < data_new[i].size() ; j++ ){
            if ( used_variables[data_new[i][j].first  ] == 0 ){
            pr.push_back({data_new[i][j].first , data_new[i][j].second.second}) ; 
            } 
        }
        vector<pair<float  , int>>pos ; 
        for (int k = 0 ; k< pr.size()-1 ; k++){
         float   first = pr[k].first ; 
         float second  = pr[k+1].first ; 
         float mid  = ( first + second)/2 ; 
         pos.push_back({mid , k+1 }) ; 
        }
       get_perfect(pr,pos,target_things ) ; 
       gini = smallest_gini_imp[2] ;  
        if (gini < gini_impurity){
            gini_impurity = gini ; 
            num = smallest_gini_imp[0] ; 
            variable_pos = (float)i ;
            thresh_data_loc = smallest_gini_imp[1] ; 
            gini_left = smallest_gini_imp[3] ; 
            gini_right = smallest_gini_imp[4] ; 
            left_leaf = smallest_gini_imp[5] ; 
            right_leaf = smallest_gini_imp[6] ; 
        }
    }
    final_ans[0] = gini_impurity ;
    final_ans[1] = num;
    final_ans[2] = variable_pos ;
    final_ans[3] = thresh_data_loc;
    final_ans[4] = gini_right;
    final_ans[5] = gini_left;

 leaf_info[num] = {left_leaf, right_leaf};
}


void get_perfect( vector<pair<float , float>> &pr , const vector<pair<float , int>>& pos , const vector<float> & target_things){


    int m = pr.size() ; 
    int n = target_things.size() ; 

    float num = 0.0  ; 
    float gini = 100 ; 
    int position  = 0  ; 
    float gini_end = 0.0  ; 
    float gini_cmp ; 
    float gini_left  = 0.0 ;  
    float gini_right = 0.0;
    float left_leaf = 0.0 ; 
    float right_leaf = 0.0 ; 
    

    if (pos.size() == 0) {
        smallest_gini_imp[0] = 0.0 ;
        smallest_gini_imp[1] = 0;
        smallest_gini_imp[2] = 100.0;
        smallest_gini_imp[3] = 0.0;
        smallest_gini_imp[4] = 0.0;
        smallest_gini_imp[5] = 0.0;
        smallest_gini_imp[6] = 0.0;
        return  ; 
    }


    unordered_map<float, int> target_index;
    for (int i = 0; i < n; i++) {
     target_index[target_things[i]] = i;
    }

    for (int i = 0 ; i < m ; i++ ){
     hash_right[target_index[pr[i].second]] = hash_right[target_index[pr[i].second]] + 1.0 ; 
    }
    hash_left.clear() ; 
    hash_right.clear() ; 
    for (int i = 0 ; i< pos.size() ; i++ ){
     int g = pos[i].second ; 
     hash_left[target_index[pr[g].second]] = hash_left[target_index[pr[g].second]] + 1.0 ; 
     hash_right[target_index[pr[g].second]] = hash_right[target_index[pr[g].second]] - 1.0 ; 
     gini_impurity_tree(hash_right  , hash_left) ; 
     gini_cmp = ans[0] ; 
     if (gini > gini_cmp) {
      gini = gini_cmp ;
      num = pos[i].first  ; 
      position = pos[i].second ; 
      gini_end = gini_cmp ; 
      gini_left = ans[1] ; 
      gini_right = ans[2] ; 
      left_leaf = target_things[(int)ans[4]] ; 
      right_leaf = target_things[(int)ans[3]] ; 
     }
    }
        smallest_gini_imp[0] = num ;
        smallest_gini_imp[1] = position;
        smallest_gini_imp[2] = gini_end ;
        smallest_gini_imp[3] = gini_left;
        smallest_gini_imp[4] = gini_right;
        smallest_gini_imp[5] = left_leaf;
        smallest_gini_imp[6] = right_leaf;

}


pair<vector<vector<pair<float,pair<float,float>>>>, vector<vector<pair<float,pair<float,float>>>>>new_dataset( const vector<float>& prev_data  ,const vector<vector<pair<float , pair<float,float>>>> & data) {
    int on_basis = (int)prev_data[2];
    int thresh = (int)prev_data[3];
    int m = data[0].size() ; 
    unordered_map<float , int>left_index ; 
    unordered_map<float , int >right_index ; 
    for (int i = 0 ; i <= thresh ; i++ ){
        left_index[data[on_basis][i].second.first]++ ; 
    }
    for (int i = thresh+1 ; i<  m ; i++){
        right_index[data[on_basis][i].second.first]++ ; 
    }
    vector<vector<pair<float,pair<float,float>>>>left_data ; 
    vector<vector<pair<float,pair<float,float>>>>right_data ; 
    left_data.resize(data.size());   
    right_data.resize(data.size());
    for (int i = 0 ; i < data.size() ; i++ ){
        for (int j = 0 ; j < data[i].size() ; j++ ){
            pair<float,pair<float,float>>vec = {data[i][j].first , {data[i][j].second.first ,data[i][j].second.second}} ; 
            if (left_index[data[i][j].second.first] > 0 ){
                left_data[i].push_back(vec) ; 
            }
            else {
                 right_data[i].push_back(vec) ; 
        }
    }
    } 
    return  {left_data,right_data} ; 
}


pair<TreeNode* , TreeNode* > rec(  unordered_map<float , pair<float , float>>leaf_info , vector<vector<pair<float , pair<float,float>>>>  dta , int i  , int n  , TreeNode*tree , TreeNode* index ){
    if (i > 5 || n == 0 || dta.empty() || dta[0].empty()){
        return {tree , index} ; 
    }
    perfect_variable(leaf_info , dta) ; 
    tree->value = final_ans[1] ; 
    index->value = final_ans[2] ; 
    used_variables[final_ans[2]]++ ; 
    pair<vector<vector<pair<float,pair<float,float>>>>, vector<vector<pair<float,pair<float,float>>>>>data_sm  = new_dataset(final_ans  , dta);
    vector<vector<pair<float , pair<float,float>>>> left_part = data_sm.first  ; 
    vector<vector<pair<float , pair<float,float>>>> right_part = data_sm.second ; 
    if (final_ans[4] > 0.01){
    tree->left = new TreeNode();
    index->left = new TreeNode() ; 
    rec(leaf_info , left_part , i+1 , n-1 ,  tree->left , index->left) ;
    }
    if (final_ans[5] > 0.01){ 
     tree->right = new TreeNode();
     index->right = new TreeNode() ; 
    rec(leaf_info , right_part , i+1 , n-1 , tree->right , index->right) ; 
    }
    return  {tree , index} ; 
}


pair<TreeNode* , TreeNode*> decision_trees(const vector<vector<pair<float , pair<float,float>>>>  & data_new  ){
    unordered_map<float , pair<float , float>>leaf_info;
    TreeNode* tree = new TreeNode() ; 
    TreeNode* index = new TreeNode() ; 
    int i  = 0 ; 
    int size = data_new[0].size() ; 
    rec(leaf_info , data_new , i , size , tree  , index ) ; 
    leaf(tree , leaf_info) ; 
    return {tree , index} ; 
}

void leaf(TreeNode*tree  ,  unordered_map<float , pair<float , float>> leaf_info ){
    if (tree == nullptr  ){
        return ; 
    }
    if (tree->leaf_status_left != nullptr && tree->leaf_status_right != nullptr){
        return ; 
    }
    if (tree->left == nullptr && tree->leaf_status_left == nullptr ){
      tree->left = new TreeNode() ; 
      float val = leaf_info[tree->value].first ; 
      tree->left->value = val ; 
     tree->leaf_status_left = new float(1.0);
    }
     if  (tree->right ==  nullptr && tree->leaf_status_right == nullptr){
     tree->right = new TreeNode() ; 
      float val = leaf_info[tree->value].second ; 
      tree->right->value = val ;  
      tree->leaf_status_right = new float(1.0);
    }
     if (tree->left != nullptr){
     leaf (tree->left , leaf_info) ;
     }
     if (tree->right != nullptr){
     leaf (tree->right , leaf_info) ;
     }
}



vector<vector<pair<float , pair<float,float>>>> sorted_dataset(const vector<vector<float>> & data  ){
    int n = data.size() ; 
    vector<vector<pair<float , pair<float,float>>>>data_info(n-1) ;
    int m = data[0].size() ; 
    float num ;
    float index ; 
    float target ; 
    pair<float,float>temp ; 
    pair<float,pair<float,float>>temp_data ; 

    for (int i = 0 ;  i< n-1 ; i++){
        data_info[i].reserve(m) ; 
        for (int j = 0 ; j < m ; j++ ){
            num = data[i][j] ; 
            target = data[n-1][j] ; 
             index = (float)j ; 
            temp = {index , target} ; 
            temp_data = {num,temp} ; 
            data_info[i].push_back(std::move(temp_data)) ; 
        }
        sort(data_info[i].begin() , data_info[i].end()) ;
    }
    return data_info ; 
}

void unique_target(const vector<vector<float>>& data){
int n = data.size() ; 
target_things.clear() ; 
set<float>sett(data[n-1].begin(), data[n-1].end());
target_things = sett ; 
}







float funtion(TreeNode*tree ,TreeNode*index ,  const vector<float> & test_data  ){
    int feature_index = (int) index->value;
    float data = test_data[feature_index];
     if (tree->left == nullptr && tree->right == nullptr && index == nullptr){
        return tree->value  ; 
     }
     if (data >= tree->value  ){
         return funtion(tree->left ,index->left ,  test_data    ) ; 
     }
     else {
         return funtion(tree->right , index->right , test_data   ) ; 
     }
}

vector<float> answer(TreeNode*tree , TreeNode* index , const vector<vector<float>>&test_data){
    vector<float>ans ; 
    ans.reserve(test_data.size()) ; 
    for (int i = 0 ; i < test_data.size() ; i++){
      vector<float>temp_test_data = test_data[i] ; 
      ans.push_back(funtion(tree , index , temp_test_data  )) ; 
    }
    return ans ; 
}

void prepare_data(){
    vector<string> training_data_title;
    set<string> test_data_lookup; 
    vector<string> common_in_both; 
    
    ifstream train_data_file("training_data.csv");
    string line;
    while(getline(train_data_file, line)) {
        training_data_title.push_back(line); 
    }
    train_data_file.close();
    
    ifstream test_data_file("testing_data.csv");
    string column;
    while(getline(test_data_file, column)){
        test_data_lookup.insert(column); 
    }
    test_data_file.close();
    
    for(string row : training_data_title) {
        if(test_data_lookup.count(row)) common_in_both.push_back(row);
    }
    
    ofstream out1("training_data.csv");
    ofstream out2("testing_data.csv");
    for(string r : common_in_both) {
        out1 << r << endl;
        out2 << r << endl;
    }
    out1.close();
    out2.close();
}

vector<float> random_forest(){
    vector<vector<float>>train_data = training_data() ; 
    vector<vector<float>>test_data = testing_data() ; 
    int vairables  = train_data.size() ; 
    unordered_map<float,int>ans ; 
    vector<vector<pair<float , pair<float,float>>>>  data_new = sorted_dataset(train_data) ; 
    unique_target(train_data) ; 
    unordered_map<float , int>hash ; 
    int no_of_trees = 15 ; 
    int i = 0 ; 
    int lower_limit ; 
    int upper_limit ; 
    vector<vector<vector<pair<float,pair<float,float>>>> >data ; 
    while (i < no_of_trees){
     lower_limit = sqrt(train_data[0].size()) ; 
     upper_limit = train_data[0].size() ;  
     int no_of_variables = rand() % (upper_limit - lower_limit + 1) + lower_limit ; 
     vector<int>set_variables ; 
      while (no_of_variables >  0 ){
          int col = rand() % (upper_limit)   ; 
          if (hash[col] == 0 ){
           set_variables.push_back(col) ; 
           no_of_variables-- ; 
           hash[col]++ ;
          }
      }

      upper_limit = train_data[0].size()  ; 
      vector<vector<pair<float,pair<float,float>>>> data_for_descion_trees_temp ; 
      vector<vector<pair<float,pair<float,float>>>> data_for_descion_trees; 
      data_for_descion_trees.reserve(data_new[0].size());  
      for (int i = 0 ; i < set_variables.size() ; i++ ){
        data_for_descion_trees_temp.push_back(std::move(data_new[set_variables[i]])) ; 
      }
      for (int i = 0 ; i < data_new[0].size() ; i++ ){
        int boot_row = rand() % data_new[0].size();
        vector<pair<float,pair<float,float>>>temp ; 
        temp.reserve(set_variables.size()) ; 
        for (int j = 0  ; j < set_variables.size() ; j++ ){
        temp.push_back(data_for_descion_trees_temp[j][boot_row]) ; 
        }
        data_for_descion_trees.push_back(temp) ; 
      }
      data.push_back(data_for_descion_trees) ; 
    i++ ; 
    }


    vector<vector<float>> pred_ans;
    pred_ans.reserve(data.size());
    #pragma omp parallel for 
    for (int i = 0 ; i < data.size() ; i++){
        pair<TreeNode* , TreeNode*> tree = decision_trees(data[i]) ; 
        vector<float>ans = answer(tree.first , tree.second , test_data) ;
        #pragma omp critical
        {
           pred_ans.push_back(ans);
        }
        delete tree.first;   
        delete tree.second;
    }
    
    vector<float>random_forest_classifier_answer;
    random_forest_classifier_answer.reserve(test_data.size());
    for (int j = 0; j < test_data.size(); j++){  
        unordered_map<float, int>hash_data;
        int maxi = -1;
        float ran = 0.0;
        
        for (int i = 0; i < pred_ans.size(); i++){  
            hash_data[pred_ans[i][j]]++;
            if (hash_data[pred_ans[i][j]] > maxi){
                maxi = hash_data[pred_ans[i][j]];
                ran = pred_ans[i][j];
            }
        }
        random_forest_classifier_answer.push_back(ran);
    }
    return random_forest_classifier_answer ; 
}

int main(){
   vector<float>ans =  random_forest() ; 
   for (int i = 0; i < ans.size() ; i++ ){
    cout<< ans[i] << " "; 
   }
   return 0 ; 
}
