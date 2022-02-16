/*
Mihriban Nur Kocak
150180090
24.11.2021
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cctype>
using namespace std;

//Function Prototypes
string toLowerCase(string convertLowercase);

//A class is constructed for corresponding input data
class Tweets
{
	int tweet_id;				  //Enumeration of the Tweet
	string tweet_datetime;		  //Tweet time in a structured format
	int tweet_unixtime;			  //Tweet time in Unix format
	string artist_name;			  //Artist name of the song
	string artist_name_lowercase; //All characters of artist name is lowercase for comparison
	string track_title;			  //Name of the song
	string country;				  //Tweet location
	string country_lowercase;	  //All characters of country is lowercase for comparison

public:
	//constructor with parameters
	Tweets(int tweet_id_given, string tweet_datetime_given, int tweet_unixtime_given, string artist_name_given, string track_title_given, string country_given)
	{
		tweet_id = tweet_id_given;
		tweet_datetime = tweet_datetime_given;
		tweet_unixtime = tweet_unixtime_given;
		artist_name = artist_name_given;
		artist_name_lowercase = toLowerCase(artist_name); //Convert all characters of artist name is lowercase for comparison
		track_title = track_title_given;
		country = country_given;
		country_lowercase = toLowerCase(country); //Convert all characters of country is lowercase for comparison
	}
	//constructor without parameters, initilizations are done using meaningless values
	Tweets()
	{
		tweet_id = 0;
		tweet_datetime = "0";
		tweet_unixtime = 0;
		artist_name = "0";
		track_title = "0";
		country = "0";
	}

	//Getters to reach private attributes
	int get_tweet_id()
	{
		return tweet_id;
	}

	string get_tweet_datetime()
	{
		return tweet_datetime;
	}

	int get_tweet_unixtime()
	{
		return tweet_unixtime;
	}

	string get_artist_name()
	{
		return artist_name;
	}

	string get_artist_name_lowercase()
	{
		return artist_name_lowercase;
	}

	string get_track_title()
	{
		return track_title;
	}

	string get_country()
	{
		return country;
	}

	string get_country_lowercase()
	{
		return country_lowercase;
	}
};

//Function Prototypes
int partition(Tweets **arr, int p, int r);
void quicksort_randomized(Tweets **arr, int p, int r);
void quicksort_deterministic(Tweets **arr, int p, int r);

//A function which is converting every character of given string to its lowercase form
//This function is used for case insensitive string comparision
string toLowerCase(string convertLowercase)
{
	int size = convertLowercase.size();
	for (int i = 0; i < size; i++)
	{
		if (isalpha(convertLowercase[i])) //if corresponding character in alphabet
		{
			convertLowercase[i] = tolower(convertLowercase[i]);
		}
	}
	return convertLowercase;
}

void quicksort_deterministic(Tweets **arr, int p, int r)
{
	//pivot is always last element in deterministic quick sort
	if (p < r) //checks the termination of the algorithm
	{
		//q is the position of the pivot
		int q = partition(arr, p, r);
		//recursive call for leftside part from partition excluding pivot
		quicksort_deterministic(arr, p, q - 1);
		//recursive call for rightside part from partition excluding pivot
		quicksort_deterministic(arr, q + 1, r);
	}
}

void quicksort_randomized(Tweets **arr, int p, int r)
{
	//to make random selection random
	if (p < r)
	{
		//select random pivot
		//int random_number = rand();
		int randomized_pivot = rand() % (r - p + 1) + p;
		//cout << "p: " << p << " r: " << r << " Pivot : " << randomized_pivot << endl;
		//move random pivot to the end of the array by swapping A[location_of_pivot] and A[r]
		Tweets *buffer = arr[randomized_pivot];
		arr[randomized_pivot] = arr[r];
		arr[r] = buffer;
		//q is the position of the pivot
		int q = partition(arr, p, r);
		//recursive call for leftside part from partition excluding pivot
		quicksort_randomized(arr, p, q - 1);
		//recursive call for rightside part from partition excluding pivot
		quicksort_randomized(arr, q + 1, r);
	}
}

//Partition splits array into two parts
//As a result of the partition, left side A[p...q-1] part contains values smaller than or equal to pivot A[q] and right side A[q+1...r] part contains values bigger than pivot A[q]
//In this algorithm, the comparison is done firstly according to countries, secondly according to artist names, thirdly according to unixtime
int partition(Tweets **arr, int p, int r)
{
	Tweets *pivot = arr[r]; //pivot is already placed at the end of the array
	int i = p - 1;			//i is the indicator of the leftside part of the partition
	for (int j = p; j < r; j++)
	{
		//To make case insensitive comparison, strings should be converted to their lowercase versions
		//string country1 = toLowerCase(arr[j]->get_country());
		//string country2 = toLowerCase(pivot->get_country());
		//int compare_countries = compareStrings(arr[j]->get_country(), pivot->get_country());
		if (arr[j]->get_country_lowercase() < pivot->get_country_lowercase())
		{
			i++;
			//swap A[i] and A[j]
			Tweets *buffer = arr[i];
			arr[i] = arr[j];
			arr[j] = buffer;
		}
		else if (arr[j]->get_country_lowercase() == pivot->get_country_lowercase())
		{
			//To make case insensitive comparison, strings should be converted to their lowercase versions
			//string artist_name1 = toLowerCase(arr[j]->get_artist_name());
			//string artist_name2 = toLowerCase(pivot->get_artist_name());
			//int compare_artists = compareStrings(arr[j]->get_artist_name(), pivot->get_artist_name());
			if (arr[j]->get_artist_name_lowercase() < pivot->get_artist_name_lowercase())
			{
				i++;
				//swap A[i] and A[j]
				Tweets *buffer = arr[i];
				arr[i] = arr[j];
				arr[j] = buffer;
			}
			else if (arr[j]->get_artist_name_lowercase() == pivot->get_artist_name_lowercase())
			{
				if (arr[j]->get_tweet_unixtime() <= pivot->get_tweet_unixtime())
				{
					i++;
					//swap A[i] and A[j]
					Tweets *buffer = arr[i];
					arr[i] = arr[j];
					arr[j] = buffer;
				}
			}
		}
	}
	//swap A[i+1] and A[r] to put pivot in its place
	Tweets *buffer = arr[i + 1];
	arr[i + 1] = arr[r];
	arr[r] = buffer;
	//return position of the pivot
	return i + 1;
}

int main(int argc, char *argv[])
{

	//first element of argv is number of tweets to be sorted
	const int num_of_tweets = stoi(argv[1]); //??we should convert char* to integer
	//second element of argv is name of the pivot selection algorithm(deterministic or randomized)
	string algorithm_type = argv[2];
	//third element of argv is input file path
	string input_file_name = argv[3];
	//fourth element of argv is output file path to save the result
	string output_file_name = argv[4];

	//open the input file
	ifstream input_file(input_file_name);
	/*
	//to check whether file is opened or not
	if (!input_file.is_open())
		throw runtime_error("Could not open file");
	*/
	//read the first line which contains colum names
	string first_line;
	getline(input_file, first_line);
	//we need an array which holds Tweets objects for each row of the data, so space allocation for this array
	Tweets **all_tweets = new Tweets *[num_of_tweets];
	//read the input file as much as desired number of tweets
	for (int i = 0; i < num_of_tweets; i++)
	{
		string data_tweet_id_string;
		string data_tweet_datetime;
		string data_tweet_unixtime_string;
		string data_artist_name;
		string data_track_title;
		string data_country;
		getline(input_file, data_tweet_id_string, '\t');			//all column in input file is seperated by \t
		int data_tweet_id = stoi(data_tweet_id_string);				//string data should be converted to integer
		getline(input_file, data_tweet_datetime, '\t');				//all column in input file is seperated by \t
		getline(input_file, data_tweet_unixtime_string, '\t');		//all column in input file is seperated by \t
		int data_tweet_unixtime = stoi(data_tweet_unixtime_string); //string data should be converted to integer
		getline(input_file, data_artist_name, '\t');				//all column in input file is seperated by \t
		getline(input_file, data_track_title, '\t');				//all column in input file is seperated by \t
		getline(input_file, data_country);							//read remaining data for the last column

		//space allocation for each row data using constructor with parameters
		Tweets *tweet = new Tweets(data_tweet_id, data_tweet_datetime, data_tweet_unixtime, data_artist_name, data_track_title, data_country);
		all_tweets[i] = tweet;

		/* For debugging purposes
		cout << all_tweets[i]->get_tweet_id() << endl;
		cout << all_tweets[i]->get_tweet_datetime() << endl;
		cout << all_tweets[i]->get_tweet_unixtime() << endl;
		cout << all_tweets[i]->get_artist_name() << endl;
		cout << all_tweets[i]->get_track_title() << endl;
		cout << all_tweets[i]->get_country() << endl;
		*/
	}

	//to calculate execution time
	clock_t execution_time;
	if (algorithm_type == "deterministic")
	{
		//if desired algorithm type is deterministic
		//cout << "Deterministic sorting started for " << num_of_tweets << " data" << endl;
		//starting time
		execution_time = clock();
		//at the begining p = 0 which is beginning of array and r = num_of_tweets - 1 which is last element of array
		quicksort_deterministic(all_tweets, 0, num_of_tweets - 1);
		//difference between starting time and ending time
		execution_time = clock() - execution_time;
	}
	else if (algorithm_type == "randomized")
	{
		srand(time(NULL));
		//if desired algorithm type is randomized
		//cout << "Randomized sorting started for " << num_of_tweets << " data" << endl;
		//starting time
		execution_time = clock();
		//at the begining p = 0 which is beginning of array and r = num_of_tweets - 1 which is last element of array
		quicksort_randomized(all_tweets, 0, num_of_tweets - 1);
		//difference between starting time and ending time
		execution_time = clock() - execution_time;
	}

	//open the output file
	ofstream output_file(output_file_name);
	//write first line of the input file which contains name of the columns to output file
	output_file << first_line << endl;
	for (int i = 0; i < num_of_tweets; i++)
	{
		output_file << all_tweets[i]->get_tweet_id() << '\t';
		output_file << all_tweets[i]->get_tweet_datetime() << '\t';
		output_file << all_tweets[i]->get_tweet_unixtime() << '\t';
		output_file << all_tweets[i]->get_artist_name() << '\t';
		output_file << all_tweets[i]->get_track_title() << '\t';
		output_file << all_tweets[i]->get_country() << endl;
	}

	//calculated clock time is converted to milisecond using CLOCKS_PER_SEC macro
	cout << "Sorted in " << (float(execution_time) / CLOCKS_PER_SEC) * 1000 << " miliseconds using " << algorithm_type << " pivot selection." << endl;

	//deallocate the allocated space for array
	for (int i = 0; i < num_of_tweets; i++)
	{
		delete all_tweets[i];
	}
	delete[] all_tweets;
	return 0;
}