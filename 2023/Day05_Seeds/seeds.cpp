#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<ctype.h>
#include<string.h>
#include<vector>
#include<chrono>

struct map_range{
    long long start;
    long long end;
    long long range;
};

void get_locations(std::vector<long long>, std::vector<long long>*, std::vector<std::vector<map_range>>);
long long get_seed_location(long long, std::vector<std::vector<map_range>>);
void get_maps(std::ifstream*, std::vector<std::vector<map_range>>*);
void get_seeds(std::ifstream *, std::vector<long long>*);
long long get_minimum_location(std::vector<long long>);
long long get_seed_range_location(std::pair<long long, long long>, std::vector<std::vector<map_range>>);

int main() {

    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream input;
    input.open("./input.txt");
    std::vector<long long> seeds, locations, locations_ranges;
    std::vector<std::vector<map_range>> maps;
    std::string line;
    long long minimum_on_first_seed_range;

    get_seeds(&input, &seeds);
    get_maps(&input, &maps);
    get_locations(seeds, &locations, maps);
    std::cout << get_minimum_location(locations) << std::endl;

    std::pair<long long, long long> seed_pair;
    for (int i = 0; i < seeds.size(); i += 2) {
        seed_pair.first = seeds[i];
        seed_pair.second = seeds[i + 1] - 1;
        minimum_on_first_seed_range = get_seed_range_location(seed_pair, maps);
        locations_ranges.push_back(minimum_on_first_seed_range);
        
    }
    std::cout << get_minimum_location(locations_ranges) << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Print the duration in seconds
    std::cout << "Execution time: " << duration.count() << " seconds." << std::endl;

    return 0;
}
//first part
void get_locations(std::vector<long long> seeds, std::vector<long long>* locations, std::vector<std::vector<map_range>> maps) {

    for (int i = 0; i < seeds.size(); i++) {
        (*locations).push_back(get_seed_location(seeds[i], maps));
    }

    return;
}

long long get_seed_location(long long seed, std::vector<std::vector<map_range>> maps) {

    long long location = seed;

    for (int i = 0; i < maps.size(); i++) {
        for (int j = 0; j < maps[i].size(); j++) {
            if(location >= maps[i][j].end && location <= (maps[i][j].end + maps[i][j].range - 1)) {
                location = maps[i][j].start + (location - maps[i][j].end);
                break;
            }
        }
    }

    return location;
}

long long get_minimum_location(std::vector<long long> locations) {

    long long minimum = LLONG_MAX;

    for (int i = 0; i < locations.size(); i++) {
        if(locations[i] < minimum) {
            minimum = locations[i];
        }
    }

        return minimum;
}

void get_maps(std::ifstream* input, std::vector<std::vector<map_range>>* maps) {

    std::string line, word;
    std::vector<map_range> map;
    map_range get_map_range;
    std::getline(*input, line);

    while(std::getline(*input, line)) {

        std::stringstream ss(line);
        if(isdigit(line[0])) {
            ss >> word;
            get_map_range.start = stoll(word);
            ss >> word;
            get_map_range.end = stoll(word);
            ss >> word;
            get_map_range.range = stoll(word);
            map.push_back(get_map_range);
        }
        if(line == "") {
            (*maps).push_back(map);
            map.clear();
        }

    }
    (*maps).push_back(map);

    return;
}

void get_seeds(std::ifstream* input, std::vector<long long>* seeds) {

    std::string line, word;
    std::getline(*input, line);
    std::stringstream ss(line);

    while(ss >> word) {
        if(isdigit(word[0])) {
            (*seeds).push_back(stoll(word));
        }
    }

    return;
}

//second part
long long get_seed_range_location(std::pair<long long, long long> seed_range, std::vector<std::vector<map_range>> maps) {

    long long minimum_of_range = LLONG_MAX;
    std::vector<std::pair<long long, long long>> locations, temp_locations;
    std::pair<long long, long long> temp_range;
    bool is_overlap_right, is_overlap_left, is_overlap_both_sides, is_overlap_full;
    long long start_seed, end_seed, range_seed, start_map, end_map, range_map;

    locations.push_back(seed_range);

    for (int i = 0; i < maps.size(); i++) {
        for (int j = 0; j < maps[i].size(); j++) {
            for (int k = 0; k < locations.size(); k++) {
                start_seed = locations[k].first;
                end_seed = locations[k].first + locations[k].second - 1;
                range_seed = locations[k].second;
                start_map = maps[i][j].end;
                end_map = maps[i][j].end + maps[i][j].range - 1;
                range_map = maps[i][j].range;
                is_overlap_both_sides = (start_seed < start_map) && (end_seed > end_map);
                is_overlap_full = (start_seed >= start_map) && (end_seed <= end_map);
                is_overlap_right = (start_seed < start_map) && (end_seed >= start_map);
                is_overlap_left = (start_seed <= end_map) && (end_seed > end_map);

                if(is_overlap_full) {
                    temp_range.first = maps[i][j].start + (start_seed - start_map);
                    temp_range.second = range_seed;
                    temp_locations.push_back(temp_range);
                    locations.erase(locations.begin() + k);
                    k--;
                } else if(is_overlap_both_sides) {
                    temp_range.first = maps[i][j].start;
                    temp_range.second = range_map;
                    temp_locations.push_back(temp_range);
                    temp_range.first = start_seed;
                    temp_range.second = start_map - (start_seed + 1);
                    locations.push_back(temp_range);
                    temp_range.first = end_map + 1;
                    temp_range.second = end_seed - (end_map + 1);
                    locations.push_back(temp_range);
                    locations.erase(locations.begin() + k);
                    k--;
                } else if(is_overlap_right) {
                    temp_range.first = maps[i][j].start;
                    temp_range.second = (end_seed - start_map);
                    temp_locations.push_back(temp_range);
                    temp_range.first = start_seed;
                    temp_range.second = start_map - (start_seed + 1);
                    locations.push_back(temp_range);
                    locations.erase(locations.begin() + k);
                    k--;
                } else if(is_overlap_left) {
                    temp_range.first = maps[i][j].start + (start_seed - start_map);
                    temp_range.second = end_map - start_seed;
                    temp_locations.push_back(temp_range);
                    temp_range.first = end_map + 1;
                    temp_range.second = end_seed - (end_map + 1);
                    locations.push_back(temp_range);
                    locations.erase(locations.begin() + k);
                    k--;
                }
            }
        }
        for (int i = 0; i < temp_locations.size(); i++) {
            locations.push_back(temp_locations[i]);
        }
        temp_locations.clear();
    }

    for (int i = 0; i < locations.size(); i++) {
        
        if(locations[i].first < minimum_of_range) {
            minimum_of_range = locations[i].first;
        }
    }

    return minimum_of_range;
}

