#ifndef SET_STATES_H
#define SET_STATES_H
#include <set>
class SetStates {
    private:
        int id;
        std::set<int> primary_ids;
        std::set<int> all_ids;
    public:
        inline static int cntSetStates = 0;
        SetStates(std::set<int> _primary_ids, std::set<int> _all_ids) : id(cntSetStates++), primary_ids(_primary_ids), all_ids(_all_ids) {}
        std::set<int> & getAll_ids() {
            return all_ids;
        }
};

#endif