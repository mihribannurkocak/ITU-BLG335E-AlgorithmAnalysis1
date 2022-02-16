/*
Mihriban Nur Kocak
150180090
17.12.2021
*/
//To do not effect the execution time I didn't use the vector from STL
//If you want to execute code with different amount of vehicles or requests, please update these macro values
#define TOTAL_VEHICLE_COUNT 1642
#define TOTAL_REQUEST_COUNT 43877
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
using namespace std;

//Vehicle object for initially available vehicles data
class Vehicle{
    int id; //unique id of vehicles
    string location; //location code where the vehicle is placed initially
    double distance; //distance to hotel from current location in kilometers
    int speed; //average speed of the vehicle in km/h
    double key; //calculated key value which is the total time needs to be spent to reach the hotel: distance / speed

    public:
    //Constructor with parameters
    Vehicle(int given_id, string given_location, double given_distance, int given_speed){
        id = given_id;
        location = given_location;
        distance = given_distance;
        speed = given_speed;
        key = distance / speed;
    }

    //Constructor without parameters, initilizations are done using meaningless values
    Vehicle(){
        id = 0;
        location = "0";
        distance = 0.0;
        speed = 0;
        key = -1;
    }

    //Getters to reach private attributes
    int get_id(){
        return id;
    }

    string get_location(){
        return location;
    }

    double get_distance(){
        return distance;
    }

    int get_speed(){
        return speed;
    }

    double get_key(){
        return key;
    }

    //Setters to set private attributes
    void set_id(int given_id){
        id = given_id;
    }    

    void set_location(string given_location){
        location = given_location;
    }

    void set_distance(double given_distance){
        distance = given_distance;
    }

    void set_speed(int given_speed){
        speed = given_speed;
    }

    void set_key(double given_key){
        key = given_key;
    }
};

//Request object for transportation requests coming from the customers
class Request{
    string location; //destination location
    double distance; //distance of the destination location to the hotel
    int luckyNumber; //lucky number of customer if any: luckyNumber = 0 means customer doesn't have lucky number
    
    //Constructor with parameters
    public:
    Request(string given_location, double given_distance, int given_luckyNumber){
        location = given_location;
        distance = given_distance;
        luckyNumber = given_luckyNumber;
    }

    //Constructor without parameters, initilizations are done using meaningless values
    Request(){
        location = "0";
        distance = 0.0;
        luckyNumber = 0;
    }

    //Getters to reach private attributes
    string get_location(){
        return location;
    }
    double get_distance(){
        return distance;
    }
    int get_luckyNumber(){
        return luckyNumber;
    }
};

//To maintain the heap property. Goal is putting (index)th element in the correct place in a portion of the array
void minHeapify(Vehicle** arr, int index, int* vehicle_heap_size){
    int leftchild = 2 * (index+1); //using formula to get index of left child
    leftchild = leftchild - 1;
    //since array index starts from 0, 1 is added to index for proper calculation then substracted from calculated value : 0's left child is 1
    int rightchild = (2 * (index+1)) + 1; //using formula to get index of right child
    rightchild = rightchild - 1;
    //since array index starts from 0, 1 is added to index for proper calculation then substracted from calculated value : 0's right child is 2
    int smallest = index; //assume index of the heap's smallest element is current index

    //if both index of left child is smaller than heap size and left child's key is smaller than its parent's key, leftchild becomes smallest
    if(leftchild < *vehicle_heap_size && arr[leftchild]->get_key() < arr[index]->get_key()){
        smallest = leftchild;
    }

    
    //if both index of rightchild is smaller than heap size and rightchild's key is smaller than both parent's key and leftchild's key, rightchild becomes smallest
    if(rightchild < *vehicle_heap_size && arr[rightchild]->get_key() < arr[smallest]->get_key()){
        smallest = rightchild;
    }

    //if smallest of parent,leftchild,rightchild is not parent, exchange parent with the smallest child
    if(smallest != index){
        Vehicle* buffer = arr[index];
        arr[index] = arr[smallest];
        arr[smallest] = buffer;
        //call minHeapify for smallest's index to maintain the heap property
        minHeapify(arr,smallest,vehicle_heap_size);
    }
}

//To convert an array into a heap using minHeapify() in a bottom-up manner
void buildMinHeap(Vehicle** arr, int* vehicle_heap_size){
    int i = (*vehicle_heap_size/2)-1; //to find max index of an interior node
    //substract 1 from pseudocode formula since the first index of an array is 0
    
    //call minHeapify for all interior nodes to maintain the heap property
    while(i >= 0){
        minHeapify(arr,i,vehicle_heap_size);
        i--;
    }
}

//To remove and return element of array with smallest key by decreasing heap size by 1
Vehicle* heapExtractMin(Vehicle** arr, int* vehicle_heap_size){
    if(*vehicle_heap_size < 1){
        //Heap size should not be lower than 1
        cout << "Heap underflow" << endl;
    }
    Vehicle* min = arr[0]; //as a heap property element with smallest key is always at 0th index 
    arr[0] = arr[(*vehicle_heap_size)-1];//move last element of the array to 0th index
    *vehicle_heap_size = *vehicle_heap_size - 1;//to remove smallest element from heap decrease heap size by 1
    minHeapify(arr,0,vehicle_heap_size);//call minHeapify for element at 0th index to maintain the heap property
    return min;//return smallest element
}

//Decrease key value of element at (index)th index to desired key value 
void heapDecreaseKey(Vehicle** arr, double key, int index){
    if(key > arr[index]->get_key()){
        //to decrease key the given key value should be smaller than current key value 
        cout << "New key is larger than current key";
    }
    arr[index]->set_key(key);//set key value of element at (index)th index to given key value
    int parent = ((index+1)/2)-1;//use formula to reach parent of the (index)th node
    while(index > 0 && arr[parent]->get_key() > arr[index]->get_key()){
        //if parent's key value is larger than (index)th node's key value, then exchange them to maintain the heap property
        //continue this operation until index becomes zero or parent's key value is smaller than or equal to (index)th node's key value
        Vehicle* buffer = arr[parent];
        arr[parent] = arr[index];
        arr[index] = buffer;
        index = parent;
        parent = ((index+1)/2)-1;
    }
}

//To call vehicles according to lucky numbers stated at request
//Count total operations for extract, decrease and insert 
Vehicle* callVehicle(Request** all_requests, Vehicle** arr, int index, int* vehicle_heap_size, int* total_operations){
    if(*total_operations == 0){
        return NULL; //if the program reached to total operation limit, return NULL to indicate termination
    }
    
    int luckyNumber = all_requests[index]->get_luckyNumber();//get luckyNumber of corresponding request

    if(luckyNumber > 0){
        //if luckyNumber is not equal to 0, to call vehicle at (luckyNumber)th index, decrease its key as (smallest-1) to make it smallest element of the heap
        heapDecreaseKey(arr, arr[0]->get_key()-1, luckyNumber);
        *total_operations = *total_operations - 1;  //Count total operations for extract, decrease and insert 
    }
    if(*total_operations == 0){
        return NULL;//if the program reached to total operation limit, return NULL to indicate termination
    }

    //extract vehicle with smallest key since it is called by customer
    Vehicle* min_vehicle = heapExtractMin(arr, vehicle_heap_size);
    *total_operations = *total_operations - 1; //Count total operations for extract, decrease and insert 

    //return called vehicle
    return min_vehicle;
}

bool minHeapInsert(Vehicle** arr, Vehicle* insert_vehicle, int* vehicle_heap_size, int* total_operations){
    //Even if insert operation has decrease operation in it, i have counted them as 1 count since insert covers decrease operation
    *total_operations = *total_operations - 1; //Count total operations for extract, decrease and insert 
    *vehicle_heap_size = *vehicle_heap_size + 1; //for insertion increase heap size by 1
    arr[*vehicle_heap_size-1] = insert_vehicle; //insert to end of the array
    if(*total_operations == 0){
        return false;//if the program reached to total operation limit, return false to indicate termination
    }
    double new_key = arr[*vehicle_heap_size-1]->get_distance() / arr[*vehicle_heap_size-1]->get_speed();
    heapDecreaseKey(arr,new_key,*vehicle_heap_size-1); //to put inserted element to correct place while maintaining the heap property
    return true;
}

int main(int argc, char *argv[]){

    int total_operations = stoi(argv[1]); //read total operation given from terminal
    int* total_operations_ptr = &total_operations; //to update total operation value via functions, pass it as pointer
	
    string vehicles_file_name = "vehicles.txt"; //name of the file which has vehicle data
    ifstream vehicles_file(vehicles_file_name); //open file which has vehicle data
    string first_line; 
    getline(vehicles_file,first_line); //read first line which is not usefull for us
    int vehicle_heap_size = TOTAL_VEHICLE_COUNT; //total number of vehicles is 1642 according to vehicle data file
    int* vehicle_heap_size_ptr = &vehicle_heap_size; //to update vehicle heap size value via functions, pass it as pointer

    Vehicle** all_vehicles = new Vehicle *[vehicle_heap_size];//Memory allocation of array which will be converted to minimum priority queue which consist of Vehicle objects
    //Read vehicle data from file to all_vehicles array
    for(int i = 0; i < vehicle_heap_size; i++){
        string data_vehicle_id_string;
        string data_vehicle_location;
        string data_vehicle_distance_string;
        string data_vehicle_speed_string;
        getline(vehicles_file, data_vehicle_id_string,'\t');
        int data_vehicle_id = stoi(data_vehicle_id_string);//convert string to integer
        getline(vehicles_file, data_vehicle_location,'\t');
        getline(vehicles_file, data_vehicle_distance_string,'\t');
        double data_vehicle_distance = stod(data_vehicle_distance_string);//convert string to double
        getline(vehicles_file, data_vehicle_speed_string);
        int data_vehicle_speed = stoi(data_vehicle_speed_string);//convert string to integer
        Vehicle* vehicle = new Vehicle(data_vehicle_id,data_vehicle_location,data_vehicle_distance,data_vehicle_speed);//Memory allocation for each Vehicle object
        all_vehicles[i] = vehicle;//Put constructed Vehicle object to all_vehicles array
    }

    //call buildMinHeap to convert all_vehicles array to minimum priority queue
    buildMinHeap(all_vehicles, vehicle_heap_size_ptr);

    string requests_file_name = "requests.txt";//name of the file which has request data
    ifstream requests_file(requests_file_name);//open file which has vehicle data
    string first_line2; 
    getline(requests_file,first_line2);//read first line which is not usefull for us
    int total_requests = TOTAL_REQUEST_COUNT;//total number of requests is 1642 according to vehicle data file

    Request** all_requests = new Request *[total_requests]; //Memory allocation of array which consist of Request objects
    //Read vehicle data from file to all_requests array
    for(int i = 0; i < total_requests; i++){
        string data_request_location;
        string data_request_distance_string;
        string data_request_luckyNumber_string;
        getline(requests_file, data_request_location,'\t');
        getline(requests_file, data_request_distance_string,'\t');
        double data_request_distance = stod(data_request_distance_string);//convert string to double
        getline(requests_file, data_request_luckyNumber_string);
        int data_request_luckyNumber = stoi(data_request_luckyNumber_string);//convert string to int
        Request* request = new Request(data_request_location, data_request_distance, data_request_luckyNumber);//Memory allocation for each Reuqest object
        all_requests[i] = request;//Put constructed Request object to all_requests array
    }

    string call_history_file_name = "call_history.txt";//name of the file which call history will be written
    ofstream call_history_file(call_history_file_name);//open file to write call history

    
    int all_called_vehicle_ids[total_requests];//array to hold called vehicles ids to write them into call history at the end
    int called_vehicle_count = 0;//to count total called vehicles 
    clock_t execution_time = clock();//get time
    for(int i = 0; i < total_requests; i++){
        Request* request = all_requests[i];//for a request in all_requests
        Vehicle* called_vehicle = callVehicle(all_requests,all_vehicles,i, vehicle_heap_size_ptr, total_operations_ptr);//call vehicle according to request and get called vehicle
        if(called_vehicle == NULL){
            break; //if program reached total operation limit after callVehicle, exit from the loop
        }
        all_called_vehicle_ids[i] = called_vehicle->get_id();//store id of called vehicle in all_called_vehicle_ids array
        called_vehicle_count++;//increase called vehicle count by 1
        called_vehicle->set_location(request->get_location());//to insert called vehicle to heap, set its location as corresponding request's location
        called_vehicle->set_distance(request->get_distance());//to insert called vehicle to heap, set its distance as corresponding request's location
        called_vehicle->set_key(10000000000000000.0);//to insert called vehicle to heap, set its key value very big to represent pozitive infinity
        bool move_loop = minHeapInsert(all_vehicles, called_vehicle, vehicle_heap_size_ptr, total_operations_ptr);//insert called vehicle to heap
        if(!move_loop){
            break;//if program reached total operation limit, exit from the loop
        }
    }
    execution_time = clock() - execution_time;//calculate spended time
    cout << "Elapsed execution time: " << (float(execution_time) / CLOCKS_PER_SEC) * 1000 << " miliseconds"<<  endl;
    
    //write id of the of the called vehicles to call history file
    for(int i = 0; i < called_vehicle_count; i++){
        call_history_file << all_called_vehicle_ids[i] << endl;
    }
    
    //deallocate the allocated space for array
	for (int i = 0; i < TOTAL_VEHICLE_COUNT; i++)
	{
		delete all_vehicles[i];
	}
	delete[] all_vehicles;

    //deallocate the allocated space for array
	for (int i = 0; i < TOTAL_REQUEST_COUNT; i++)
	{
		delete all_requests[i];
	}
	delete[] all_requests;

    //close files
    vehicles_file.close();
    requests_file.close();
    call_history_file.close();

    return 0;
}