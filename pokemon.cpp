//Project Identifier:  = 5949F553E20B650AB0FB2266D3C0822B13D248B0
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;


class Pokemon{
    private:
        string mode = "";
        size_t v_num = 0;
        struct Coordinate{
            int coord_x;
            int coord_y;
            char terrain;
            Coordinate(): coord_x(0), coord_y(0), terrain(' '){}
            Coordinate(int x, int y, char t): coord_x(x), coord_y(y), terrain(t){}
        };

        struct Prim{
            bool k;
            double d;
            uint64_t p;
            Prim(): k(false), d(numeric_limits<double>::infinity()), p(0){}
            Prim(bool k, double d, uint64_t p): k(k), d(d), p(p){}
        };
        vector<Coordinate> coords;
        vector<Prim> prim_table;
        vector<uint32_t> tsp_table;
        
        void printHelp(char *argv[]) {
            cout << "Usage: " << argv[0] << " -m, --mode {MST|FASTTSP|OPTTSP}\n";
            cout << "This program is to help you become a pokemon master. Gotta Catch 'Em All!\n";
        } 

        double calculateDist(Coordinate &a, Coordinate &b){
            if((a.terrain=='l'&&b.terrain=='w')||(a.terrain=='w'&&b.terrain=='l')){
                return numeric_limits<double>::infinity();
            }
            return static_cast<double>(a.coord_x-b.coord_x)*static_cast<double>(a.coord_x-b.coord_x)+static_cast<double>(a.coord_y-b.coord_y)*static_cast<double>(a.coord_y-b.coord_y);
        }

        double calculateDistB(Coordinate &a, Coordinate &b){
            return sqrt(static_cast<double>(a.coord_x-b.coord_x)*static_cast<double>(a.coord_x-b.coord_x)+static_cast<double>(a.coord_y-b.coord_y)*static_cast<double>(a.coord_y-b.coord_y));
        }

        double calculateDistC(Coordinate &a, Coordinate &b){
            return static_cast<double>(a.coord_x-b.coord_x)*static_cast<double>(a.coord_x-b.coord_x)+static_cast<double>(a.coord_y-b.coord_y)*static_cast<double>(a.coord_y-b.coord_y);
        }

        double calculateDistD(uint32_t a, uint32_t b){
            double edge;
            if(a > b){
                uint32_t c = a;
                a = b;
                b = c;
            }
            if(edges_table[a][b]==0){
                edge = sqrt(static_cast<double>(coords[a].coord_x-coords[b].coord_x)*static_cast<double>(coords[a].coord_x-coords[b].coord_x)+static_cast<double>(coords[a].coord_y-coords[b].coord_y)*static_cast<double>(coords[a].coord_y-coords[b].coord_y));
                edges_table[a][b]= edge;
            }
            else{
                edge = edges_table[a][b];
            }
            return edge;
        }

        double best_weight;
        vector<uint32_t> best_path;
        double current_weight = 0;
        vector<uint32_t> current_path;
        vector<vector<double>> edges_table;

        double calculateTSP(size_t permLength){
            double new_weight = 0;
            if(permLength==1){
                current_weight = 0;
            }
            else{
                new_weight = calculateDistD(current_path[permLength-2], current_path[permLength-1]);
                current_weight+= new_weight;
            }
            return new_weight;
        }


        double calculateMST(vector<uint32_t> &path){
            vector<Coordinate> sub_coords;
            sub_coords.reserve(path.size());
            for(size_t i=0; i<path.size(); i++){
                sub_coords.push_back(coords[path[i]]);
            }
            vector<Prim> prim_table;
            prim_table.resize(path.size());
            prim_table[0].d = 0;
            
            size_t current = 0;
            
            for(size_t j = 0; j < path.size(); j++){
                double min = numeric_limits<double>::infinity();
                for(size_t i = 0; i < path.size(); i++){
                    if(prim_table[i].k == false&&prim_table[i].d<min){
                        min = prim_table[i].d;
                        current = i;
                    }
                }
                prim_table[current].k = true;
                for(size_t o = 0; o < prim_table.size(); o++){
                    if(prim_table[o].k==false){
                        double new_dist = calculateDistC(sub_coords[current], sub_coords[o]);
                        if(new_dist<prim_table[o].d){
                            prim_table[o].p = current;
                            prim_table[o].d = new_dist;
                        }
                    }
                }
            }
            double total_weight = 0;
            for(size_t n = 1; n < path.size(); n++){
                total_weight += sqrt(prim_table[n].d);
            }
            return total_weight;
        }

        bool promising(size_t permLength){
            vector<uint32_t> part2(current_path.begin() + static_cast<int>(permLength), current_path.end());
            double MST = calculateMST(part2);
            double minarm1 = numeric_limits<double>::infinity();
            double minarm2 = numeric_limits<double>::infinity();
            double arm1 = 0;
            double arm2 = 0;
            
            if(current_weight==0){
                for(size_t i = 0; i < part2.size(); i++){
                    arm1 = calculateDistD(part2[i], current_path[0]);
                    if(arm1<minarm1){
                        minarm1 = arm1;
                    }
                }
                minarm2 = minarm1;
            }
            else{
                for(size_t i = 0; i < part2.size(); i++){
                    arm1 = calculateDistD(part2[i], current_path[0]);
                    arm2 = calculateDistD(part2[i], current_path[permLength-1]);
                    if(arm1<minarm1){
                        minarm1 = arm1;
                    }
                    if(arm2<minarm2){
                        minarm2 = arm2;
                    }
                }
            }         
            return (minarm1+minarm2+MST+current_weight) < best_weight;
        }

        void genPerms(size_t permLength) {
            if (permLength == v_num) {   
                double last_edge = calculateDistD(current_path[v_num-1], current_path[0]);
                current_weight += last_edge;
                if(current_weight<best_weight){
                    best_path = current_path;
                    best_weight = current_weight;
                }
                current_weight -= last_edge;
                return;
            } 
            if (!promising(permLength)) {
                return;
            } 
            for (size_t i = permLength; i < v_num; ++i) {
                swap(current_path[permLength], current_path[i]);
                double new_weight = calculateTSP(permLength+1);
                genPerms(permLength + 1);
                current_weight -= new_weight;
                swap(current_path[permLength], current_path[i]);
            } 
        } 

        
        void partC(){
            current_path.resize(v_num);
            iota(current_path.begin(), current_path.end(), 0);
            edges_table.resize(v_num, vector<double>(v_num, 0));
            best_weight = partB();
            genPerms(1);
            cout << best_weight << "\n";
            for(size_t i=0; i<v_num; i++){
                cout << best_path[i] << " ";
            }
        }

        void partA(){
            prim_table.resize(v_num);
            prim_table[0].d = 0;
            size_t current = 0;
            for(size_t j = 0; j < v_num; j++){
                double min = numeric_limits<double>::infinity();
                for(size_t i = 0; i < v_num; i++){
                    if(prim_table[i].k == false&&prim_table[i].d<min){
                        min = prim_table[i].d;
                        current = i;
                    }
                }
                prim_table[current].k = true;
                for(size_t o = 0; o < v_num; o++){
                    if(prim_table[o].k==false){
                        double new_dist = calculateDist(coords[current], coords[o]);
                        if(new_dist<prim_table[o].d){
                            prim_table[o].p = current;
                            prim_table[o].d = new_dist;
                        }
                    }
                }
            }
            double total_weight = 0;
            for(size_t n = 1; n < v_num; n++){
                total_weight += sqrt(prim_table[n].d);
            }
            cout << total_weight << "\n";
            for(size_t i = 1; i < v_num; i++){
                if(i < prim_table[i].p){
                    cout << i << " " << prim_table[i].p << "\n";
                }
                else{
                    cout << prim_table[i].p << " " << i << "\n";
                }
            }
        }

        double partB(){
            tsp_table.reserve(v_num);
            tsp_table.push_back(0);
            tsp_table.push_back(1);
            tsp_table.push_back(2);
            double total_weight = 0;
            for(size_t i = 3; i< v_num; i++){
                double min = numeric_limits<double>::infinity();
                size_t index = 1;
                for(size_t j = 0; j<tsp_table.size(); j++){
                    double distance;
                    if(j == tsp_table.size()-1){
                        distance = calculateDistB(coords[i], coords[tsp_table[j]]) +  calculateDistB(coords[i], coords[tsp_table[0]])-calculateDistB(coords[tsp_table[j]],  coords[tsp_table[0]]);
                    }
                    else{
                        distance = calculateDistB(coords[i], coords[tsp_table[j]]) +  calculateDistB(coords[i], coords[tsp_table[j+1]])-calculateDistB(coords[tsp_table[j]],  coords[tsp_table[j+1]]);
                    }
                    if(distance<min){
                        min = distance;
                        index = j+1;
                    }
                }
                tsp_table.insert(tsp_table.begin() + static_cast<int>(index), static_cast<uint32_t>(i));
            }
            for(size_t i = 0; i<v_num-1; i++){
                total_weight += calculateDistB(coords[tsp_table[i]], coords[tsp_table[i+1]]);
            }
            total_weight += calculateDistB(coords[tsp_table[v_num-1]], coords[tsp_table[0]]);

            if(mode == "FASTTSP"){
                cout << total_weight << "\n";
                for(size_t i=0; i<v_num; i++){
                    cout << tsp_table[i] << " ";
                }
            }
            best_path = tsp_table;
            current_path = tsp_table;
            return total_weight;
        }

    public:
        void getMode(int argc, char * argv[]) {
            opterr = false; // Let us handle all error output for command line options
            int choice;
            int index = 0;
            option long_options[] = {
                {"help", no_argument, 0, 'h'},
                {"mode", required_argument, 0, 'm'},
                {0, 0, 0, 0},
            }; 
            if (argc <= 1) {
                cerr << "Usage: " << argv[0] << " -h, --help | -m, --mode {MST|FASTTSP|OPTTSP}\n";
                exit(1);
            }
            while ((choice = getopt_long(argc, argv, "hm:", long_options, &index)) != -1) {
                switch (choice) {
                    case 'h':
                        printHelp(argv);
                        exit(0);
                    case 'm':
                        mode = optarg;
                        break;
                    default:
                        cerr << "Usage: " << argv[0] << " -h, --help | -m, --mode {MST|FASTTSP|OPTTSP}\n";
                        exit(1);
                }
            }
        }
        void readInput(){
            cin >> v_num;
            coords.reserve(v_num);
            bool water = false;
            bool land = false;
            bool coast = false;
            int x_coord;
            int y_coord;
            char terrain;
            if(mode == "MST"){
                while(cin >>x_coord>>y_coord){
                    if((x_coord == 0 && y_coord <= 0) || ( x_coord <= 0 && y_coord == 0)){
                        terrain = 'c';
                        coast = true;
                    }
                    else if(x_coord<0&&y_coord<0){
                        terrain = 'w';
                        water = true;
                    }
                    else{
                        terrain = 'l';
                        land = true;
                    }
                    coords.push_back({x_coord, y_coord, terrain});
                }
                if(water&&land&&!coast){
                    cerr << "Cannot construct MST";
                    exit(1);
                }
            }
            else if(mode == "FASTTSP"||mode == "OPTTSP"){
                while(cin >>x_coord>>y_coord){
                    coords.push_back({x_coord, y_coord, 'c'});
                }
            }
            else{
                cerr << "Unrecognized mode\n";
                exit(1);
            }

            
        }

        void process(){
            if(mode=="MST"){
                partA();
            }
            else if(mode=="FASTTSP"){
                partB();
            }
            else if(mode == "OPTTSP"){
                partC();
            }
        }
};

int main(int argc, char *argv[]) {
    cout << setprecision(2); //Always show 2 decimal places
    cout << fixed; //Disable scientific notation for large numbers
    std::ios_base::sync_with_stdio(false);
    Pokemon pokemon;
    pokemon.getMode(argc, argv);
    pokemon.readInput();
    pokemon.process();
}