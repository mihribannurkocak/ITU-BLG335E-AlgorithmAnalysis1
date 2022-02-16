/*
Mihriban Nur Kocak
150180090
05.01.2021
*/
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <string.h>
using namespace std;

//Class for node of the tree represents Video Games data 
class VideoGame{
    //private attributes
    bool is_nill; //holds whether node is nill or not
    int year_of_release; //release year 
    string publisher; //name of the publisher
    float na_sales; //amount of North America sales
    float eu_sales; //amount of Europe sales
    float other_sales; //amount of Other sales
    VideoGame* right; //right child
    VideoGame* left; //left child
    VideoGame* parent; //parent
    int color; //0 means black, 1 means red
    
    public:
    //Constructor with parameters
    VideoGame(bool given_is_nill,int given_year_of_release=0, string given_publisher="-", float given_na_sales=0.0, float given_eu_sales=0.0, float given_other_sales=0.0){
        is_nill = given_is_nill;
        year_of_release = given_year_of_release;
        publisher = given_publisher;
        na_sales = given_na_sales;
        eu_sales = given_eu_sales;
        other_sales = given_other_sales;
        right = NULL;
        left = NULL;
        parent = NULL;
        if(is_nill) color = 0; //if nill set black
        else color = 1; //if not set initially red
    }

    //Getters to reach private attributes
    int get_year_of_release(){
        return year_of_release;
    }

    string get_key(){
        return publisher;
    }

    float get_na_sales(){
        return na_sales;
    }
    
    float get_eu_sales(){
        return eu_sales;
    }
    
    float get_other_sales(){
        return other_sales;
    }

    VideoGame* get_right(){
        return right;
    }

    VideoGame* get_left(){
        return left;
    }

    VideoGame* get_parent(){
        return parent;
    }

    //to learn color of the node
    bool is_black(){
        if(color == 0) return true;
        else return false;
    }

    bool is_red(){
        if(color == 1) return true;
        else return false;
    }

    //to learn whether node is nill or not
    bool get_is_nill(){
        return is_nill;
    }

    //Setters to update private attributes
    void set_year_of_release(int given){
        year_of_release = given;
    }

    void set_publisher(string given){
        publisher = given;
    }

    void set_na_sales(float given){
        na_sales = given;
    }
    
    void set_eu_sales(float given){
        eu_sales = given;
    }
    
    void set_other_sales(float given){
        other_sales = given;
    }

    void set_right(VideoGame* given){
        right = given;
    }

    void set_left(VideoGame* given){
        left = given;
    }

    void set_parent(VideoGame* given){
        parent = given;
    }

    void set_color_black(){
        color = 0;
    }
    
    void set_color_red(){
        color = 1;
    }

    //to print color of the node
    string print_color(){
        if(color == 0) return "(BLACK) ";
        else return "(RED) ";
    }
};

//Class represent Red Black Tree whose nodes are Video Games data
class RBTree{
    //private attributes
    VideoGame* root; //node of the tree
    float best_seller_na; //amount of the bestseller's sales from Nort America
    string best_seller_na_name; //the bestseller's name from Nort America
    float best_seller_eu; //amount of the bestseller's sales from Europe
    string best_seller_eu_name; //the bestseller's name from Europe
    float best_seller_other; //amount of the bestseller's sales from Other
    string best_seller_other_name; //the bestseller's name from Other

    public:
    //default constructor
    RBTree(){
        root = NULL; //initially there is no root
        best_seller_na = -1.0; //set minimum value
        best_seller_eu = -1.0; //set minimum value
        best_seller_other = -1.0; //set minimum value
    }

    //default deconstructor
    ~RBTree(){
        if(root != NULL){
            //if tree is not empty
            VideoGame* first_nill = root->get_parent(); //also parent has a nill node as its parent
            post_order_delete(root); //secure deletion is possible only with post order traversal
            delete first_nill; //delete parent's parent node which is nill node
        }
    }

    //getters to reach private attributes
    VideoGame* get_root(){
        return root;   
    }

    void post_order_delete(VideoGame* cursor){
        //deletion is possible only with post order traversal since the parent of the children shouldn't be deleted before children are deleted
        //recursive calls
        if(cursor != NULL){
            post_order_delete(cursor->get_left());
            post_order_delete(cursor->get_right());
            delete cursor;
            cursor = NULL;
        }
    }

    //Left rotate with respect to given node
    void leftRotate(VideoGame* to_rotate){
        //cout << "Left rotate " <<to_rotate->get_key() << endl;
        VideoGame* current_right = to_rotate->get_right();
        to_rotate->set_right(current_right->get_left());//set given's right as, given's right's left
        to_rotate->get_right()->set_parent(to_rotate);//set given's new right's parent as given
        current_right->set_parent(to_rotate->get_parent());//set old right's parent as given's parent
        if(to_rotate->get_parent()->get_is_nill()){
            //if given is root
            //update root as given's old right
            root = current_right;
        }
        else{
            //if given is not root
            if(to_rotate == to_rotate->get_parent()->get_left()){
                //if given is its parent's left child
                to_rotate->get_parent()->set_left(current_right); //change its parent left child as its right child
            }
            else{
                //if given is its parent's right child
                to_rotate->get_parent()->set_right(current_right); //change its parent right child as its right child
            }
        }
        current_right->set_left(to_rotate);//set given's right child's left child as given
        to_rotate->set_parent(current_right);//set given's parent as given's old right child
    }

    //Right rotate with respect to given node
    //Everything is same as Left Rotate only exchange left and right operations                
    void rightRotate(VideoGame* to_rotate){
        //cout << "Right rotate " <<to_rotate->get_key() << endl;
        VideoGame* current_left = to_rotate->get_left();
        to_rotate->set_left(current_left->get_right());
        to_rotate->get_left()->set_parent(to_rotate);
        current_left->set_parent(to_rotate->get_parent());
        if(to_rotate->get_parent()->get_is_nill()){
            root = current_left;
        }
        else{
            if(to_rotate == to_rotate->get_parent()->get_right()){
                to_rotate->get_parent()->set_right(current_left);
            }
            else{
                to_rotate->get_parent()->set_left(current_left);
            }
        }

        current_left->set_right(to_rotate);
        to_rotate->set_parent(current_left);
    }

    //For fixing up to protect Red Black tree property after insertion to tree
    void insert_fixup(VideoGame* to_fixup){
        while(to_fixup->get_parent()->is_red()){
            //execute until fixup's parent becomes black
                //PART 1: If fixup's parent is fixup's grandparent's left child
                if(to_fixup->get_parent() == to_fixup->get_parent()->get_parent()->get_left()){
                    VideoGame* grandparents_right_child = to_fixup->get_parent()->get_parent()->get_right();
                    if(grandparents_right_child->is_red()){
                        //case1: if fixup's uncle is red
                        //cout << "CASE1.1" << endl;
                        to_fixup->get_parent()->set_color_black(); //set fixup's parent as black
                        grandparents_right_child->set_color_black(); //set fixup's uncle as black
                        to_fixup->get_parent()->get_parent()->set_color_red(); //set fixup's grandparent as red
                        to_fixup = to_fixup->get_parent()->get_parent(); //move by fixing up fixup's grandparent
                    }
                    else{
                        //if fixup's uncle is black
                        if(to_fixup == to_fixup->get_parent()->get_right()){
                            //case2: if fixup is its parent's right child
                            //cout << "CASE1.2" << endl;
                            to_fixup = to_fixup->get_parent(); //move by fixing up fixup's parent
                            leftRotate(to_fixup); //left rotate with respect to new fixup to make red black tree balanced 
                        }
                        //case3: if fixup's uncle is black
                        //cout << "CASE1.3" << endl;
                        to_fixup->get_parent()->set_color_black(); //set fixup's parent as black
                        to_fixup->get_parent()->get_parent()->set_color_red(); //set fixup's grandparent as red
                        rightRotate(to_fixup->get_parent()->get_parent()); //right rotate with respect to fixup's grandparent to make red black tree balanced
                    }
                }
                //PART 2: If fixup's parent is fixup's grandparent's right child
                //Everything is same as PART1 only exchange left and right operations                
                else{
                    VideoGame* grandparents_left_child = to_fixup->get_parent()->get_parent()->get_left();
                    if(grandparents_left_child->is_red()){
                        //case1
                        //cout << "CASE2.1" << endl;
                        to_fixup->get_parent()->set_color_black();
                        grandparents_left_child->set_color_black();
                        to_fixup->get_parent()->get_parent()->set_color_red();
                        to_fixup = to_fixup->get_parent()->get_parent();
                    }
                    else{
                        if(to_fixup == to_fixup->get_parent()->get_left()){
                            //case2
                            //cout << "CASE2.2" << endl;
                            to_fixup = to_fixup->get_parent();
                            rightRotate(to_fixup);
                        }
                        //case3
                        //cout << "CASE2.3" << endl;
                        to_fixup->get_parent()->set_color_black();
                        to_fixup->get_parent()->get_parent()->set_color_red();
                        leftRotate(to_fixup->get_parent()->get_parent());
                    }
                }
        }
        root->set_color_black(); //set color of the root as black every time since it is the property of Red black tree
    }

    //To construct the tree by inserting nodes
    void insert(VideoGame* to_insert){
        VideoGame* cursor = root; //set cursor as root initially
        if(cursor == NULL){
            //if tree is empty
            root = to_insert; //set new node as the root
            root->set_parent(new VideoGame(true)); //set new node's parent as nill
            root->get_parent()->set_left(root); //set parent's left as new node
            root->get_parent()->set_right(NULL); //set parent's right as NULL
            root->get_parent()->set_parent(NULL); //set parent's parent as NULL
            root->set_right(new VideoGame(true)); //set new node's right child as nill
            root->get_right()->set_parent(root); //set right child's parent as new node
            root->set_left(new VideoGame(true)); //set new node's left child as nill
            root->get_left()->set_parent(root); //set left child's parent as new node
            insert_fixup(root); //call fix up for new node to protect red black tree property
            return; //finnish the function call
        }
        //if tree is not empty
        while(!cursor->get_is_nill()){
            //until cursor is nill which means cursor is in the place where it will insert new node to there
            //to find correct place for node
            if(to_insert->get_key() < cursor->get_key()){
                //if new node's key is less than the cursor's key, go left with cursor
                if(cursor->get_left()->get_is_nill()){
                    //cursor is at the correct place for insertion since it reached to nill node
                    VideoGame* current_left = cursor->get_left();
                    /*
                    Cursor's left child is nill node
                    Insert new node between cursor's left child(nill node) and cursor
                    Set new node's right child as new nill node
                    */
                    to_insert->set_left(current_left);
                    current_left->set_parent(to_insert);
                    to_insert->set_parent(cursor);
                    cursor->set_left(to_insert);
                    to_insert->set_right(new VideoGame(true));
                    to_insert->get_right()->set_parent(to_insert);
                    insert_fixup(cursor->get_left()); //call fix up for new node to protect red black tree property
                    break; //finnish since insertion is done
                }
                else{
                    //continue search for correct place by going left with cursor
                    cursor = cursor->get_left();
                }
            }
            else if(to_insert->get_key() > cursor->get_key()){
                //if new node's key is greater than the cursor's key, go right with cursor                
                if(cursor->get_right()->get_is_nill()){
                    //cursor is at the correct place for insertion since it reached to nill node
                    VideoGame* current_right = cursor->get_right();
                    /*
                    Cursor's right child is nill node
                    Insert new node between cursor's right child(nill node) and cursor
                    Set new node's left child as new nill node
                    */
                    to_insert->set_right(current_right);
                    current_right->set_parent(to_insert);
                    to_insert->set_parent(cursor);
                    cursor->set_right(to_insert);
                    to_insert->set_left(new VideoGame(true));
                    to_insert->get_left()->set_parent(to_insert);
                    insert_fixup(cursor->get_right());//call fix up for new node to protect red black tree property
                    break;//finnish since insertion is done
                }
                else{
                    //continue search for correct place by going right with cursor
                    cursor = cursor->get_right();
                }
            }
            else{
                //if new node's key equal the cursor's key only update amount of sales by adding, do not insert to node               
                cursor->set_na_sales(cursor->get_na_sales() + to_insert->get_na_sales());
                cursor->set_eu_sales(cursor->get_eu_sales() + to_insert->get_eu_sales());
                cursor->set_other_sales(cursor->get_other_sales() + to_insert->get_other_sales());
                delete to_insert; //delete since the node will not be inserted
                break;//finnish since we are done
            }
        }
    }

    //To print the whole tree, sample output is written according to preorder traversal
    void preorder_traverse(VideoGame* cursor, int level){
        if(!cursor->get_is_nill()){
            for(int i = 0; i < level; i++) cout << "-";
            //print root
            cout << cursor->print_color();
            cout << cursor->get_key() << endl;
            preorder_traverse(cursor->get_left(), level+1); //call for left child which is at deeper level
            preorder_traverse(cursor->get_right(), level+1); //call for right child which is at deeper level
        }
    }

    //To find the best sellers of each decade, traverse tree with the preorder traversal
    void find_best_sellers(VideoGame* cursor){
        if(!cursor->get_is_nill()){
            if(cursor->get_na_sales() > best_seller_na){
                //if cursor's na sale amount is larger than current bestseller's, update bestseller's info
                best_seller_na = cursor->get_na_sales();
                best_seller_na_name = cursor->get_key();
            }
            if(cursor->get_eu_sales() > best_seller_eu){
                //if cursor's eu sale amount is larger than current bestseller's, update bestseller's info
                best_seller_eu = cursor->get_eu_sales();
                best_seller_eu_name = cursor->get_key();
            }
            if(cursor->get_other_sales() > best_seller_other){
                //if cursor's other sale amount is larger than current bestseller's, update bestseller's info
                best_seller_other = cursor->get_other_sales();
                best_seller_other_name = cursor->get_key();
            }
            find_best_sellers(cursor->get_left()); //call for left child
            find_best_sellers(cursor->get_right()); //call for right child
        }
    }

    //Print bestseller's info in desired format
    void print_bestsellers_info(int end_year){
        cout << "End of the " << end_year << " Year" << endl;
        cout << "Best seller in North America: " << best_seller_na_name << " - ";
        //it is indicated to use 2 digits after decimal point
        printf("%.2lf",best_seller_na); 
        cout << " million" << endl;
        cout << "Best seller in Europe: " << best_seller_eu_name << " - ";
        printf("%.2lf",best_seller_eu); 
        cout << " million" << endl;
        cout << "Best seller rest of the World: " << best_seller_other_name << " - ";
        printf("%.2lf",best_seller_other); 
        cout << " million" << endl;
    }
};

int main(int argc, char *argv[]){

    string input_file_name = argv[1];//get file name that contains data from terminal
    ifstream video_games_file(input_file_name);//open file
    string first_line;
    getline(video_games_file,first_line);//read first line as buffer
    
    RBTree rbtree;//initialize a new binary tree

    int year_to_find_best_seller = 1991;//first year when we will find the best sellers of 1980-1990 decade
    while(!video_games_file.eof()){
        //until end of the while
        string input_name_s;
        string input_platform_s;
        string input_year_of_release_s;
        string input_publisher_s;
        string input_na_sales_s;
        string input_eu_sales_s;
        string input_other_sales_s;
        //use comma as parameter since columns are seperated with comma
        getline(video_games_file, input_name_s,',');
        if(input_name_s == "") break;//do not read the last empty line existing in data file
        getline(video_games_file, input_platform_s,',');
        getline(video_games_file, input_year_of_release_s,',');
        getline(video_games_file, input_publisher_s,',');
        getline(video_games_file, input_na_sales_s,',');
        getline(video_games_file, input_eu_sales_s,',');
        getline(video_games_file, input_other_sales_s,'\n');
        int input_year_of_release = stoi(input_year_of_release_s);//convert string to integer
        float input_na_sales = stof(input_na_sales_s);//convert string to float
        float input_eu_sales = stof(input_eu_sales_s);//convert string to float
        float input_other_sales = stof(input_other_sales_s);//convert string to float
        VideoGame* video_game = new VideoGame(false,input_year_of_release, input_publisher_s, input_na_sales, input_eu_sales, input_other_sales);//create new Video Game object for readed data
        if(input_year_of_release == year_to_find_best_seller){
            //if time comes to find bestsellers
            rbtree.find_best_sellers(rbtree.get_root()); //call to traverse the tree
            rbtree.print_bestsellers_info(input_year_of_release-1); //call to print obtained info
            year_to_find_best_seller += 10; //update new year as beginning of the next decade to find best sellers when this year comes
        }
        rbtree.insert(video_game); //insert new node to tree
    }
    //since all insertions are done, print the final results
    rbtree.find_best_sellers(rbtree.get_root());
    rbtree.print_bestsellers_info(year_to_find_best_seller-1); // for end of the 2020
   
    rbtree.preorder_traverse(rbtree.get_root(), 0); //print the tree by preorder traversal starting from the root which is at level 0
    //close files
    video_games_file.close(); //close the input file

    return 0;
}