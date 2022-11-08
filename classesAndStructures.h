using namespace std;
using namespace std::chrono;

int transaction_id {1};

string get_transaction_id(){
    string temp = to_string(transaction_id);
    string zeros2 = "00" ;
    string id = zeros2 + temp;
    transaction_id++;

    return id;
}
struct Transaction{
    string transac_id;
    float total_fee;
    int total_distance;
    int total_time;
    string bought_time; 
    string customer_name;
    string departure_station;
    string arrival_station;

    Transaction* next=NULL;
    Transaction* prev=NULL;

    Transaction(string transac_id, float total_fee, int total_distance, int total_time, string bought_time, string customer_name, string departure_station, string arrival_station){
        this->transac_id = transac_id;
        this->total_fee = total_fee;
        this->total_distance = total_distance;
        this->total_time = total_time;
        this->bought_time = bought_time;
        this->customer_name = customer_name;
        this->departure_station = departure_station;
        this->arrival_station = arrival_station;
    }
    ~Transaction(){ 
        cout << "Deleted  " << this << "\n";
    }
};
class Station{
    public:
    int station_id; 
    string station_name;
    int travel_time;
    int travel_distance; 
    float fee;
    Station* next;
    Station* prev;

    Station(int station_id, string station_name, int travel_time, int travel_distance, float fee){
        this->station_id=station_id;
        this->station_name = station_name;
        this->travel_time = travel_time;
        this->travel_distance = travel_distance;
        this->fee = fee;
        this->next = NULL;
        this->prev = NULL;
    }
    Station(){
    }
    ~Station(){ 
        cout << "Deleted station " << this->station_name << " object at " << this << "\n";
    }
};
class Customer{
    public:
    string username;
    string password;
    int id;
    string fullname;
    Transaction* transaction_head_node;

    string ic;
    Customer(int id, string username, string password, string fullname, string ic){
        this->id = id;
        this->username = username;
        this->password = password;
        this->fullname = fullname;
        this->ic = ic;
        this->transaction_head_node = NULL;
    }
    // Append transaction to customer's linked list head
    void append_transaction_customer(string transac_id, float total_fee, int total_distance, int total_time, string bought_time, string departure_info, string arrival_info)
    {
        char input;
        //Transaction(float total_fee, int total_distance, int total_time, string bought_time, string customer_name, string departure_station, string arrival_station)
        Transaction* new_node = new Transaction(transac_id, total_fee, total_distance, total_time, bought_time, fullname, departure_info, arrival_info);
        Transaction** head_ref= &transaction_head_node; 
        Transaction *last = *head_ref;

        new_node->next = NULL;

        if (*head_ref == NULL){
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
    // Append to global transaction linked list head 
    void append_transaction_list(Transaction **transaction_list_head, string transac_id, float total_fee, int total_distance, int total_time, string bought_time, string departure_info, string arrival_info){

        Transaction *last2 = *transaction_list_head;
        Transaction *new_node2 = new Transaction(transac_id, total_fee, total_distance, total_time, bought_time, fullname, departure_info, arrival_info);
        new_node2->next = NULL;

        if (*transaction_list_head == NULL)
        {
            new_node2->prev = NULL;
            *transaction_list_head = new_node2;
            return;
        }
        while (last2->next != NULL)
            last2 = last2->next;

        last2->next = new_node2;
        new_node2->prev = last2;

        return;
    }
    // Prints customer's individual transactions
    void display_transactions(){
        Transaction* node = transaction_head_node; 

        if(transaction_head_node!=NULL){
            int counter {1};
            cout << "-----Transaction History-----" << endl;
            TextTable t( '-', '|', '+' );
            t.add("NO.");
            t.add("Transaction ID");
            t.add("Name");
            t.add("Departure Station");
            t.add("Arrival Station");
            t.add("Total Travel Distance" );
            t.add("Estimated Travel Time");
            t.add("Ticket Bought Date and Time");
            t.endOfRow();

            while (node != NULL){
                t.add(to_string(counter));
                t.add(node->transac_id);
                t.add(node->customer_name);
                t.add(node->departure_station);
                t.add(node->arrival_station);
                t.add(to_string(node->total_distance));
                t.add(to_string(node->total_time));
                t.add(node->bought_time);
                t.endOfRow();
                t.setAlignment(2, TextTable::Alignment::RIGHT);
                node = node->next;
                counter++;
            }
            cout << t << endl;
        }
        else{
            cout << "No Transaction History." << endl;
        }
    }
    // Prints a receipt of ticket after ticket confirmation
    void display_receipt(){
        Transaction* node = transaction_head_node;

        while (node->next!=NULL)
            node = node->next;
        
        cout << endl; 
        cout << "----------Receipt----------"   << endl;
        cout << "Transaction ID: "              << node->transac_id << endl;
        cout << "Name: "                        << node->customer_name << endl;
        cout << "Departure Station: "           << node->departure_station << endl;
        cout << "Arrival Station: "             << node->arrival_station << endl ;
        cout << "Total Travel Distance: "       << node->total_distance << endl;
        cout << "Estimated Travel Time: "       << node->total_time << endl;
        cout << "Ticket Bought Date and Time: " << node->bought_time << endl;
        cout << "-----------------------------" << endl << endl;
    }
    ~Customer(){ 
        cout << "Destroyed customer object at " << this << "\n";
    }
};
class Admin{
    

    public:
    string username = "1";
    string password = "1";
    int num_customers = 3;
    // Prints all customer's individual transactions
    void display_transactions(Customer** customers){
        int counter {1};
        for(int i=0; i < num_customers; i++){
            customers[i]->display_transactions();
        }
    }
    // Long string input is segmented by "\", "tokenized", and stored into a vector for further use. Input example: station_id/station_name/travel_time/travel_distance/travel_fee
    void tokenize(string const &str, const char delim, vector<string> &out)
    {
        // construct a stream from the string
        stringstream ss(str);
    
        string s;
        while (getline(ss, s, delim)) {
            out.push_back(s);
        }
    }
    // Finalize Edit of a whole station 
    void edit_station_everything(Station* node){
        string input;
        char input_2;
        int info = 5;
        int n = 0;
        cout << "Please type all the details in this format: station_id/station_name/travel_time/travel_distance/travel_fee " << endl;
        cout << "Input: " ;
        
        cin >> input;

        const char delim = '/' ;
        vector<string> out;
        tokenize(input, delim, out);
        
        if (out.size()!=5){
            cout << "Input is wrong. Back to main page." << endl;
            return;
        }
        else{
            cout << endl;
            cout << "                      Before----After"<< endl;
            cout << "Station ID:         " << node->station_id      << "----"   << out[0] << endl;
            cout << "Station Name:       " << node->station_name    << "----"   << out[1] << endl;
            cout << "Travel Time (next): " << node->travel_time     << "----"   << out[2] << endl;
            cout << "Travel Distance (km)" << node->travel_distance << "----"   << out[3] << endl;
            cout << "Travel Fee (RM):    " << node->fee             << "----"   << out[4] << endl;
            
            cout << "Do you wish to confirm changes? ([y]/n): " ;
            cin >> input_2;

            if (input_2=='y'){
                node->station_id = stoi(out[0]) ;
                node->station_name = out[1];
                node->travel_time = stoi(out[2]);
                node->travel_distance = stoi(out[3]);
                node->fee = stof(out[4]);
                // Code to check node's data
                // cout << node->station_id << endl;
                // cout << node->station_name << endl;
                // cout << node->travel_time << endl;
                // cout << node->travel_distance << endl;
                // cout << node->fee << endl;
                return;
            } 
            else {
                cout << "Returning to Admin Page..." << endl;
            }
        }
    }
    // Bubble sort swap function
    struct Transaction* swap(struct Transaction* ptr1, struct Transaction* ptr2)
    {
        struct Transaction* tmp = ptr2->next;
        ptr2->next = ptr1;
        ptr1->next = tmp;
        return ptr2;
    }
    // Bubble sorting list, by name in accending order
    void bubbleSort_name(struct Transaction** head, int count)
    {
        struct Transaction** h;
        int i, j, swapped;
    
        for (i = 0; i <= count; i++){
            h = head;
            swapped = 0;
            for (j = 0; j < count - i - 1; j++){
                struct Transaction* p1 = *h;
                struct Transaction* p2 = p1->next;
                if (p1->customer_name > p2->customer_name){
                    // update the link after swapping 
                    *h = swap(p1, p2);
                    swapped = 1;
                }
                h = &(*h)->next;
            }
            // break if the loop ended without any swap
            if (swapped == 0)
                break;
        }
    }
    // Bubble sorting list, by ID in accending order
    void bubbleSort_id(struct Transaction** head, int count)
    {
        struct Transaction** h;
        int i, j, swapped;
        for (i = 0; i <= count; i++){
            h = head;
            swapped = 0;
            for (j = 0; j < count - i - 1; j++){
                struct Transaction* p1 = *h;
                struct Transaction* p2 = p1->next;
                if (p1->transac_id > p2->transac_id){
                    // update the link after swapping
                    *h = swap(p1, p2);
                    swapped = 1;
                }
                h = &(*h)->next;
            }
            /* break if the loop ended without any swap */
            if (swapped == 0)
                break;
        }
    }
    // Print out list of transactions of any given transaction head pointer 
    void print_list(Transaction *transaction_head){
        int counter{1};
        Transaction *node = transaction_head;
        TextTable t( '-', '|', '+' );

        if (node != NULL){
            int counter {1};
            cout << "-----Transaction History-----" << endl;
            t.add("NO.");
            t.add("Transaction ID");
            t.add("Name");
            t.add("Departure Station");
            t.add("Arrival Station");
            t.add("Total Travel Distance" );
            t.add("Estimated Travel Time");
            t.add("Ticket Bought Date and Time");
            t.endOfRow();
        }

        else if (node==NULL){
            cout << "No Transaction History." << endl;
        }

        while (node != NULL){
            t.add(to_string(counter));
            t.add(node->transac_id);
            t.add(node->customer_name);
            t.add(node->departure_station);
            t.add(node->arrival_station);
            t.add(to_string(node->total_distance));
            t.add(to_string(node->total_time));
            t.add(node->bought_time);
            t.endOfRow();
            t.setAlignment(2, TextTable::Alignment::RIGHT);
            node = node->next;
            counter++;
        }
        cout << t << endl;
    }
    void delete_transaction(Transaction *transaction_list_head){
        string id_input;
        Transaction *temp = transaction_list_head;;
        cout << "Select transaction to delete by ID number: " ;
        cin >> id_input;
        while(temp->next!=NULL){
            if(temp->transac_id==id_input && temp->next->next!=NULL){
                cout << "Deleted Transaction " << temp->transac_id << " from Customer (" << temp->customer_name << ")" << endl;
                temp->next = temp->next->next;
            }
            temp = temp->next;
        }
    }
    ~Admin(){ 
        cout << "Destroyed Admin Object at " << this << "\n";
    }
};

