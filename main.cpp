#include <bits/stdc++.h>
#include <cstring>
#include <iostream>
#include <ctime>
#include <chrono>
#include "TextTable.h"
#include "classesAndStructures.h"
#include <vector>
#include <sstream>
#include <limits.h>
using namespace std;
using namespace std::chrono;

// Global Variables of Station head pointer, customer array pointer, transaction list head pointer
Station *head = NULL;
Customer **customers = new Customer*[3];
Transaction *transaction_list_head = NULL;

// Returns a string of current system time
string get_time(){
    string time;
    auto timenow =chrono::system_clock::to_time_t(chrono::system_clock::now());
    time = ctime(&timenow);
    time.erase(std::remove(time.begin(), time.end(), '\n'), time.end());
    return time;Transaction *main = NULL;
}

// Returns true if train station is within service hours, false otherwise
bool check_service_hours(){
    string time = get_time();
    int n = time.size();
    char char_array [n + 1];
    strcpy(char_array, time.c_str());

    // Uncomment these 4 lines to apply real time 
    int hour_tens = char_array[11] - '0';
    int hour_ones = char_array[12] - '0';
    int minute_tens = char_array[14] - '0';
    int minute_ones = char_array[15] - '0';

    // Set dummy time rather than using real time, remember to comment out when switching 
    // int hour_tens = 0;
    // int hour_ones = 0;
    // int minute_tens = 0;
    // int minute_ones = 0;

    if (hour_tens==0){
        if( hour_ones >= 1 && hour_ones < 6){
            cout << "Station Operates from 6am to 1am, please book your tickets at another time." << endl;
            cout << "Back to home page..." << endl;
            return false;
        }
        else{
            cout << "Station currently in service hour." << endl;
            return true;
        }
    }
    else{
        cout << "Station currently in service hour." << endl;
        return true;
    }
}
// Returns true if ticket estimate arrival time is within service hours, false otherwise 
bool check_ticket_time(int total_time, bool now, char* given_time){
    get_time();
    string time = get_time();
    cout << endl;
    int n = time.size();
    char char_array [n + 1];
    strcpy(char_array, time.c_str());

    // Uncomment these 4 lines to apply real time 
    int clock_hour_tens = char_array[11] - '0';
    int clock_hour_ones = char_array[12] - '0';
    int clock_minute_tens = char_array[14] - '0';
    int clock_minute_ones = char_array[15] - '0';

    // Set dummy time rather than using real time, remember to comment out when switching 
    // int clock_hour_tens = 1;
    // int clock_hour_ones = 0;
    // int clock_minute_tens = 0;
    // int clock_minute_ones = 0;

    if (now==false){
        clock_hour_tens = given_time[0] - '0';
        clock_hour_ones = given_time[1] - '0';
        clock_minute_tens = given_time[3] - '0';
        clock_minute_ones = given_time[4] - '0';
    }

    // cout << "clock_hour_tens: " << clock_hour_tens << endl;
    // cout << "clock_hour_ones: " << clock_hour_ones << endl;
    // cout << "clock_minute_tens: " << clock_minute_tens << endl; 
    // cout << "clock_minute_ones: " << clock_minute_ones << endl;
    cout << "Current Time: " << clock_hour_tens << clock_hour_ones << ":" << clock_minute_tens << clock_minute_ones << endl;
    
    //Do time addition
    int time_minute_tens = total_time / 10;
    int time_minute_ones = total_time % 10;

    int total_minute_ones = time_minute_ones + clock_minute_ones;
    int total_minute_tens = time_minute_tens + clock_minute_tens;

    if(total_minute_ones >= 10){
        total_minute_ones = total_minute_ones - 10;
        total_minute_tens += 1;
    }
    if(total_minute_tens >= 6){
        total_minute_tens = total_minute_tens - 6;
        clock_hour_ones += 1;
    }
    

    cout << "Estimated arrival time: " << clock_hour_tens << clock_hour_ones << ":" << total_minute_tens << total_minute_ones << endl;

    if (clock_hour_tens==0){
        if( clock_hour_ones >= 1 && clock_hour_ones < 6){
            cout << "Station Operates from 6am to 1am, please book your tickets at another time." << endl;
            cout << "Back to home page.." << endl;
            return false;
        }
        else{
            cout << "Station is in service. Proceed." << endl;
            return true;
        } 
    }
    else{
        cout << "Station is in service. Proceed." << endl;
        return true;
    }
}
// Add new station to the front/left of the linked list/station line
void push(Station** head_ref, int station_id, string station_name, int travel_time, int travel_distance, float fee)
{
    Station* new_node = new Station(station_id, station_name, travel_time, travel_distance, fee);

    new_node->next = (*head_ref);
    new_node->prev = NULL;
 
    if ((*head_ref) != NULL){
        (*head_ref)->prev = new_node;
    }
    (*head_ref) = new_node;
}
// Add new station to the back/right of the linked list/station line
void append(Station** head_ref, int station_id, string station_name, int travel_time, int travel_distance, float fee)
{
    Station* new_node = new Station(station_id, station_name, travel_time, travel_distance, fee);
    Station* last = *head_ref; 
    new_node->next = NULL;

    if (*head_ref == NULL)
    {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }
    while (last->next != NULL)
        last = last->next;
 
    last->next = new_node;
    new_node->prev = last;
    return;
}
// Prints a dotted line
void line(){
    cout << endl << "---------------------------------------------------------------" <<endl;
}
// Print the complete list of stations, including any station that is added.
void print_listOfStations(Station* node)
{
    cout<<"\n --><-- --><-- --><-- Station Line --><-- --><-- --><-- --><--" ;
    line();
    do{
        cout<< "(" << node->station_id << ")" <<node->station_name;
        if (node->next!=NULL){
            cout  <<" <--> ";
        }
        node = node->next;
    } while (node != NULL);
    line();
}

// Once ticket is confirmed able to be purchased, append to customer's own transaction linked list, and to global transaction_list_head for admin sorting.
void confirmAndCreate_transaction(Customer* customer, float total_fee, int total_distance, int total_time, string departure_info, string arrival_info){
    char input;
    get_time();
    string bought_time = get_time();
    string transac_id = get_transaction_id();

    cout << endl << "Do you with to confirm your ticket purchase? ([y]/n): " ;
    cin >> input;

    if (input == 'y'){
        customer->append_transaction_customer(transac_id, total_fee, total_distance, total_time, bought_time, departure_info, arrival_info);
        customer->append_transaction_list(&transaction_list_head, transac_id, total_fee, total_distance, total_time, bought_time, departure_info, arrival_info);
        customer->display_receipt();
    }
    return;
}
// Create list of default stations
auto list_of_stations(){
    Station* head = NULL;
    push(&head, 1, "Titiwangsa", 3, 4, 0.40);
    append(&head, 2, "PWTC", 7, 8, 0.80);
    append(&head, 3, "Sultan", 7, 8, 0.80);
    append(&head, 4, "Masjed Jamek",5, 6, 0.60);
    append(&head, 5, "Plaza Rakyat", 9, 10, 1.00);
    append(&head, 6, "Hang Tuah", 4, 5, 0.50);
    append(&head, 7, "Pudu", 4, 5, 0.50);
    append(&head, 8, "Chan Sow Lin", 0, 0, 0.00);
    return head;
}
// If departure choice of station id is smaller than arrival, passenger travels from left stations to right. Default Stations ID are set at 1 to 8, from left to right.
void left_to_right(Customer* customer, int departure_station, int arrival_station, bool now, char* time_for_later){
    float total_fee {0};
    int total_distance {0};
    int total_time {0};    
    string departure_info; //Details for transactions
    string arrival_info;
    Station* node = head;
    bool time_check;
    while (node != NULL){
        if(node->station_id == departure_station){
            departure_info = node->station_name;
            cout << "DEPARTURE Station: " << node->station_name << endl;
            do{
                total_fee += node->fee;
                total_distance += node->travel_distance;
                total_time += node->travel_time;

                if (node->next->station_id == arrival_station){
                    arrival_info = node->next->station_name;
                    cout << "ARRIVAL Station: " << node->next->station_name << endl;
                }
                else{
                    cout << "Stations to travel: " << node->next->station_name << endl;
                }
                node = node->next;
                
            } while (node->station_id != arrival_station);
            break;
            
        }
        node = node->next;
    }
    time_check = check_ticket_time(total_time, now, time_for_later);
    if (time_check==true){
        cout << endl << "Total Fee: RM" << total_fee << endl;
        cout << "Total Distance: " << total_distance << " km" << endl;
        cout << "Total Time: " << total_time << " minutes" << endl;
        confirmAndCreate_transaction(customer, total_fee, total_distance, total_time, departure_info, arrival_info);
    }
    return;
}
// If departure choice of station id is bigger than arrival, passenger travels from right stations to left. Default Stations ID are set at 1 to 8, from left to right.
void right_to_left(Customer* customer, int departure_station, int arrival_station, bool now, char* time_for_later){
    float total_fee {0};
    int total_distance {0};
    int total_time {0};
    bool time_check;
    string departure_info; //Details for transactions
    string arrival_info;
    Station *node = head;
    
    while (node != NULL){
        if(node->station_id == departure_station){ //Loop through linked list at the starting/head node
            departure_info = node->station_name;
            cout << "DEPARTURE Station: " << node->station_name << endl;
            do{
                total_fee += node->prev->fee;
                total_distance += node->prev->travel_distance;
                total_time += node->prev->travel_time;
                if (node->prev->station_id == arrival_station){
                    arrival_info = node->prev->station_name;
                    cout << "ARRIVAL Station: " << node->prev->station_name << endl;
                    
                }
                else{
                    cout << "Stations to travel: " << node->prev->station_name << endl;
                }
                node = node->prev;
            } while (node->station_id != arrival_station); 
            break;
            
        }
        node = node->next;
    }
    
    time_check = check_ticket_time(total_time, now, time_for_later);

    if (time_check==true){
        cout << endl << "Total Fee: RM" << total_fee << endl;
        cout << "Total Distance: " << total_distance << " km" << endl;
        cout << "Total Time: " << total_time << " minutes" << endl;

        confirmAndCreate_transaction(customer, total_fee, total_distance, total_time, departure_info, arrival_info);
    }
    return;
}

// Customer home page
void customer_home_page(Customer* customer){
    int input;

    while (true){
        cout << endl << "-----Home Page of " << customer->fullname << "-----" << endl;
        print_listOfStations(head);
        cout << endl;
        cout << "-----Select one of the following options-----" << endl;
        cout << "1. Purchase a ticket." << endl;
        cout << "2. View your transaction history." << endl;
        cout << "3. Exit to main menu. (or any other key)" << endl << endl;
        cout << "Input number: " ;
        cin >> input;
        cout << endl;

        if (input==1){
            while (true){
                cout << "1. Purchase for NOW" << endl;
                cout << "2. Purchase for LATER" << endl;
                cout << "3. Back to HOME PAGE (or any other key)" << endl << endl;
                cout << "Input: " ;
                cin >> input;
                cout << endl;
                if (input==1){
                    bool time_pass = check_service_hours();
                    if (time_pass==true){
                        int current_station {0};
                        int destination_station {0};
                        cout << "Please select your current station (Type Station Number as Input): " ;
                        cin >> current_station;
                        cout << "Please select your destination station (Type STation Number as Input): " ;
                        cin >> destination_station;
                        cout << endl;

                        if (destination_station > current_station){
                            bool now = true;
                            char* empty;
                            left_to_right(customer, current_station, destination_station, now, empty);
                        }
                        else if (destination_station < current_station){
                            bool now = true;
                            char* empty;       
                            right_to_left(customer, current_station, destination_station, now, empty);
                        }
                        else if (destination_station == current_station){
                            cout << "Same station is selected twice. Please try again." << endl;

                        }
                        continue;
                    }
                    else if(time_pass==false){
                        cout << "Train operates daily at 6am to 1am. It is not currently operational. Time now is: " << get_time() << endl; 
                    }
                }
                else if(input==2){
                    string input;
                    char y_input;
                    cout << "Please input your desire time in 24 hour format (23:95): " ;
                    cin >> input;
                    int n = input.size();
                    char char_array [n + 1];
                    strcpy(char_array, input.c_str());

                    int clock_hour_tens = char_array[0] - '0';
                    int clock_hour_ones = char_array[1] - '0';
                    int clock_minute_tens = char_array[3] - '0';
                    int clock_minute_ones = char_array[4] - '0';
                    int hours = clock_hour_tens * 10 + clock_hour_ones;
                    int minutes = clock_minute_tens * 10 + clock_minute_ones;
                    cout << "Hours: " << hours << endl;
                    cout << "Minutes: " << minutes << endl;

                    if (hours > 23 || hours <0 || minutes > 59 || minutes < 0){
                        cout << "Invalid Time Format! Please try again." << endl;
                        cout << endl;
                        continue;
                    }
                    else {
                        cout << "Confirm time: " << hours << ":" << minutes << "  ? ([y]/n): " ;
                        cin >> y_input;
                        if (y_input=='y'){
                            print_listOfStations(head);
                            cout << endl;
                            int current_station {0};
                            int destination_station {0};

                            // Customer now selects his current station and destination
                            cout << "Please select your current station (Type Station Number as Input): " ;
                            cin >> current_station;
                            cout << "Please select your destination station (Type STation Number as Input): " ;
                            cin >> destination_station;
                            cout << endl;

                            if (destination_station > current_station){
                                bool now = false;
                                left_to_right(customer, current_station, destination_station, now, char_array);
                            }
                            else if (destination_station < current_station){
                                bool now = false;
                                right_to_left(customer, current_station, destination_station, now, char_array);
                            }
                            break;
                        }
                    }
                }
                else {
                    cout << "Back to Home Page" << endl;
                    break;
                }
            }
            continue;
        }
        else if (input==2){
            customer->display_transactions();
            continue;
        }
        else if (input==3){
            return;
        }
        break;
    }
}

// Customers username and password check
void customer_login_page(Customer** customers){
    string username;
    string password;
    string input;
    int num_customers=3; //Customer array size is 3, 3 customers

    while(true){
        cout << "CUSTOMER LOGIN PAGE" << endl;
        line();
        cout << endl ;
        cout << "Please Login Your Credentials Below: " << endl;
        cout << "Username: " ; cin >> username;
        cout << "Password: "; cin >> password;
        cout << endl;

        bool match = false;
        for(int i=0; i < num_customers;i++){
            if (customers[i]->username==username && customers[i]->password==password){
                match = true;
                cout << "\n\n\n\n\n\n\n\n\n\n"; 
                customer_home_page(customers[i]);
                return;
            }
            else{
                continue;
            }
        }
        cout << match << endl;
        if (match==false){
            cout << "Login Failed. Do you wish to try again? (y/n): ";
            cin >> input;
            if (input=="y"){
                continue;
            } 
            else{
                cout << endl;
                return;
            }
        }
    }
    return;
}
// Create all customers' accounts
auto create_customers(){
    customers[0] = new Customer(1, "1", "1", "Carmen", "980405141122");
    customers[1] = new Customer(2, "2", "2", "Brian", "980405141122");
    customers[2] = new Customer(3, "3", "3", "Andrew", "980405141122");
    return customers;
}
// Delete stations to prevent memory leaks
void delete_stations(Station** head_ref){
    Station* current = *head_ref;
    Station* next = NULL;
 
    while (current != NULL){
        next = current->next;
        delete(current);
        current = next;
    }
    *head_ref = NULL;

}
// Edit every detail at once of a station page. This page asks you which station to edit and what details to fill. Calls another function within admin->edit_station_everything(node) to finalize the edit. 
void edit_station_everything_page(Admin* admin){
    int input;
    // Input type safety check. Needed because linked list type consistency is necessary.
    while (cout << "Which station would you like to edit? Input the Station ID Number: " && !(cin >> input)) {
        cin.clear(); //clear bad input flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
        cout << "Invalid input; please re-enter.\n";
    }
    cout << endl;

    Station* node = head;
    int first_id = node->station_id;
    Station* last = head;
    int last_id;

    // Check length of linked list to get the int of the last station
    while (last->next!=NULL){
        last = last->next;
    }
    last_id = last->station_id;

    // Print out the selected station details, including next and prev station details. 
    // Check if selected station is first or last to avoid printing first node's prev and last node's next as they are NULL.
    while (node!=NULL){
        if (node->station_id==input){
            // int station_id, string station_name, int travel_time, int travel_distance, float fee
            cout << "Station ID: " << node->station_id << endl;
            cout << "Station Name: " << node->station_name << endl;

            // Print next details if station selected is not last
            if (input!= last_id){
                cout << "- Travel to next Station: No." << input+1 << " " << node->next->station_name 
                     << " --- Travel Time: " << node->travel_time << "mins --- " <<  "Travel Distance: " 
                     << node->travel_distance<< " --- Travel Fee: " << node->fee << " -" << endl;
            }
            // Print prev details if station selected is not first
            if (input!=first_id){
                cout << "- Travel to prev Station: No. " << input-1 << " " << node->prev->station_name 
                << " --- Travel Time: " << node->prev->travel_time << "mins --- " << "Travel Distance: " 
                << node->prev->travel_distance<< " --- Travel Fee: " << node->prev->fee <<  " -" << endl;
            }

            cout << endl << "1. Edit " << endl << "2. Back to Admin Home Page (or any other key)" << endl;
            cout << "Input: " ;
            cin >> input;
            cout << endl;
            if (input==1){
                admin->edit_station_everything(node);
                print_listOfStations(head);
            }
            else{
                cout << "Back to main page..." << endl;
                break;
            }
        }
        node=node->next;
    }             
}
// Edit specific details of any station from station list
void edit_station_specific(Admin* admin){
    int input;
    string string_input;
    float float_input;
    while (cout << "Which station would you like to edit? Input the Station ID Number: " && !(cin >> input)) {
        cin.clear(); //clear bad input flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
        cout << "Invalid input; please re-enter.\n";
    }
    cout << endl;
    Station* node = head;
    
    while (node->station_id!=input){
        node = node->next;
    }

    cout << "------Please select which specific detail you wish to edit------" << endl;
    cout << "1. Station ID: "       << node->station_id      << endl;
    cout << "2. Station Name: "     << node->station_name    << endl;
    cout << "3. Travel Time: "      << node->travel_time     << endl;
    cout << "4. Travel Distance: "  << node->travel_distance << endl;
    cout << "5. Fee: " << node->fee << endl;
    cout << "Input: " ;
    cin >> input;
    cout << endl;

    switch(input){
        case 1: 
            cout <<  "Change Station ID from (" << node->station_id << ") to: " ;
            cin >> input;
            node->station_id = input;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');  //Switching from int type cin to float type input, ignore "\n" 
            break;
        case 2:
            cout <<  "Change Station Name from (" << node->station_name << ") to: " ;
            cin >> string_input;
            node->station_name = string_input;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');  
            break;
        case 3:
            cout <<  "Change Travel Time from (" << node->station_name << ") to: " ;
            cin >> input;
            node->travel_time = input;
            break;
        case 4:
            cout <<  "Change Travel Distance from (" << node->station_name << ") to: " ;
            cin >> input;
            node->travel_distance = input;
            break;
        case 5:
            cout <<  "Change Fee from (" << node->station_name << ") to: " ;
            cin >> input;
            node->fee = float_input;
            break;
    }
    print_listOfStations(head);
}
// Add a new station to the front/back, left/right of the station list. 
void add_station(Admin* admin){
    int add_choice=0;
    int station_id=0;
    string station_name;
    int travel_time=0;
    int travel_distance=0;
    float fee=0.0;

    cout << "1. Push a new station    (first to the left)"  << endl;
    cout << "2. Append a new station  (first to the right)" << endl;
    cout << "3. Back to Admin Home Page (or any other key)" << endl << endl;
    cout << "Select how you wish to add on new stations: " ;
    cin >> add_choice;

    if (add_choice==1||add_choice==2){
        int input;
        float float_input;
        string string_input;

        cout << endl << "---Please fill up the following Station details---" << endl << endl;
        cout << "(1/5) Station ID (,-1,-0, 1...): ";
        cin >> input;
        station_id = input;

        cout << "(2/5) Station Name: ";
        cin >> string_input;
        station_name = string_input;

        cout << "(3/5) Travel Time: ";
        cin >> input;
        travel_time = input;

        cout << "(4/5) Travel Distance: ";
        cin >> input;
        travel_distance = input;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');  //Switching from int type cin to float type input, ignore "\n" 

        cout << "(5/5) Fee (type float only): ";
        cin >> float_input;
        fee = float_input;

        cout << endl;
        cout << "------Confirmation------" << endl;
        cout << "Station ID: "      << station_id      << endl;
        cout << "Station Name: "    << station_name    << endl;
        cout << "Travel Time: "     << travel_time     << endl;
        cout << "Travel Distance: " << travel_distance << endl;
        cout << "Fee: "             << fee             << endl;

        char confirm;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout << endl;

        while (cout << "Please confirm the Station details ([y]/n): " && !(cin >> confirm)) {
                cin.clear(); //clear bad input flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
                cout << "Invalid input; please re-enter.\n";
            }

        if (confirm=='y'){
            if (add_choice==1){
                push(&head, station_id, station_name, travel_time, travel_distance, fee);
                print_listOfStations(head);
            }
            else if (add_choice==2){
                append(&head, station_id, station_name, travel_time, travel_distance, fee);
                print_listOfStations(head);
            }
        }
        else{
            cout << "Returning to Admin Page" << endl;
        }
    }
    else{
        cout << "Returning to Admin Page." << endl;
    }
}
// Login page of Admin
void admin_home_page(Admin* admin){
    int size_of_linkedlist = 0; 
    Transaction *temp = transaction_list_head;

    // Loop through transaction_list_head to get the size/length for bubble sorting
    if(transaction_list_head!=NULL){
        size_of_linkedlist=1;
        while(temp->next!=NULL){
            size_of_linkedlist++;
            temp = temp->next;
        }
    }

    bool exit = false;
    while (!exit){
        int input;
        cout << endl << endl << "--------ADMIN HOME PAGE--------" << endl;
        cout << "1. Display Every Customer's Transactions" << endl;
        cout << "2. Display Transactions (ID Sorted)" << endl;
        cout << "3. Display Transactions (Name Sorted)" << endl;
        cout << "4. Edit/Add Stations" << endl; 
        cout << "5. Edit and Delete Transaction" << endl;
        cout << "6. Exit to Main Page" << endl << endl;
        cout << "Please select one of the options by typing the number as input: ";
        cin >> input;
        cout << endl << endl;

        switch(input){
            case 1:
                admin->display_transactions(customers);
                continue;
            case 2:
                admin->bubbleSort_id(&transaction_list_head, size_of_linkedlist);
                admin->print_list(transaction_list_head);
                continue;
            case 3:
                admin->bubbleSort_name(&transaction_list_head, size_of_linkedlist);
                admin->print_list(transaction_list_head);
                continue;
            case 4:
                cout << "-----Edit/Add Stations-----" << endl;
                cout << "1. Edit a whole Station" << endl;
                cout << "2. Edit specifics of a Station" << endl;
                cout << "3. Add a Station" << endl;
                cout << "4. Back to Admin Home Page (or any other key)" << endl;
                cout << "Input: " ;
                cin >> input;
                cout << endl;
                if (input==1){
                    print_listOfStations(head);
                    edit_station_everything_page(admin);
                }
                if (input==2){
                    print_listOfStations(head);
                    edit_station_specific(admin);
                }
                else if (input==3){
                    print_listOfStations(head);
                    add_station(admin);
                }
                continue;
            case 5:
                if (transaction_list_head==NULL){
                    cout << "No Transaction History" << endl;
                    continue;
                }
                else{
                    admin->bubbleSort_id(&transaction_list_head, size_of_linkedlist); //Make sure transaction list is sort by ID, instead of name
                    admin->print_list(transaction_list_head); 
                    admin->delete_transaction(transaction_list_head);    
                }
                continue;
            case 6:
                exit = true;
                continue;
            default:
                cout << "Invalid Input. Please try again." << endl; 
        }
        continue;
    }
}
// Admin username and password check
void admin_login_page(Admin* admin){
    string username;
    string password;
    string input;
    while(true){
        cout << "ADMIN LOGIN PAGE" << endl;
        line();
        cout << endl ;
        cout << "Please Login Your Credentials Below: " << endl;
        cout << "Username: " ; cin >> username;
        cout << "Password: "; cin >> password;
        cout << endl;
        bool match = false;

        if (admin->username==username && admin->password==password){
            match = true;
            cout << "\n\n\n\n\n\n\n\n\n\n"; 
            admin_home_page(admin);
            return;
        }
        else{
            continue;
        }

        cout << match << endl;

        if (match==false){
            cout << "Login Failed. Do you wish to try again? (y/n): ";
            cin >> input;
            if (input=="y"){
                continue;
            } 
            else{
                cout << endl;
                return;
            }
        }
    }
    return;
}
int main(){
    head = list_of_stations();  // Create the default list of stations
    Admin* admin = new Admin(); // Create admin account
    customers = create_customers(); //Create customers account 
    int user_input;

    while (true){
        line();
        cout << "MAIN PAGE" << endl; 
        cout << "-----Welcome to Malaysia Subway-----" << endl;
        cout << "1. Customer Login\n2. Administrator Login\n3. Exit" << endl << endl;
        cout << "Please select one of the options by typing the number as input: ";
        cin >> user_input;

        switch (user_input){
            // Customer Login 
            case 1: 
                customer_login_page(customers);
                continue;
            // Admin Login
            case 2:
                admin_login_page(admin);
                continue;
            // Exit Program
            case 3:
                cout << "---Exiting Program---" << endl ;
                delete admin;
                delete_stations(&head);
                return 0;
            default:
                cout << "Invalid input, please try again." << endl;
                continue;
        } 
    }
    return 0;
}