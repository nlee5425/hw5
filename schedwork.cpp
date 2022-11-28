

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here


bool worker_scheduling (
    const Worker_T& worker,
    vector<vector<Worker_T>>& schedule,
    const int& day
);
void schedule_helper (
    const vector<vector<bool>>& avail,
    vector<vector<Worker_T>>& schedule,
    int occurence[],
    int& day,
    const size_t dailyNeed,
    const size_t maxShifts,
    bool& valid_schedule 
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    
    vector< Worker_T > temp;

    for (int i = 0 ;  i < (int)avail.size() ; ++i) {
        sched.push_back(temp);
    }

    int sizer = (avail[0]).size();
    int occurence[sizer];
    for ( int i = 0 ; i < sizer ; ++i ) {
        occurence[i] = 0;
    }
    bool schedule_validation = false;
    int day = 0;
    schedule_helper(avail, sched, occurence , day , dailyNeed, maxShifts, schedule_validation);
    return schedule_validation;
}

bool worker_scheduling (
    const Worker_T& currworker,
    vector<vector<Worker_T>>& schedule,
    const int& day
)
{
    if ( schedule[day].size() == 0 ) { //checking if the day has no workers scheduled
        return true;
    } 
    //if above if statement does not execute, need to iterate through everyone in the schedule and check
    for ( int i = 0 ; i < (int)schedule[day].size() ; ++i ) {
        if (schedule[day][i] == currworker ) { //if the worker is on the scheudle for the day, need to return false bc worker already exists
            return false;
        }
    }
    //if cannot find the worker, return true
    return true;


}

void schedule_helper (
    const vector<vector<bool>>& avail,
    vector<vector<Worker_T>>& schedule,
    int occurence[],
    int& day,
    const size_t dailyNeed,
    const size_t maxShifts,
    bool& schedule_validation
)
{
    for (int i = 0 ; i < (int)avail[0].size() ; ++i ) { //need to create a new worker that is called from the avail array by iterating through whole array
        Worker_T currworker = (unsigned int) i; //calling a new worker
        if (  //need to add the worker to the schedule 
            avail[day][i] == true //ensuring that the worker is available for work
            && occurence[i] < (int)maxShifts  //ensuring that the work has not reached the max amount of shifts able to work
            && worker_scheduling(currworker, schedule , day) //helper function used to identify that the worker is not in schedule
        )
        { //if all constraints pass, worker is able to be added
            ++(occurence[i]); //iterating the amount of times the worker shows up 
            schedule[day].push_back(currworker); //adding worker into the schedule
            //need to check whether or not the current day you are looking at needs workers
            if ( schedule[day].size() == dailyNeed ) { //reached max amount of people, dont need workers that day
                if (day < (int)avail.size()-1) { //if there are more days that need to be filled, continue
                    ++day;
                    schedule_helper( avail, schedule , occurence , day , dailyNeed, maxShifts ,  schedule_validation );
                    if ( schedule_validation ) { //if the scheule returns true from the recursive call, return bc schedule is complete
                        return;
                    }
                    else { //if schedule did is not validated as complete
                        --(occurence[i]); 
                        --day;
                        schedule[day].pop_back();
                    }
                }
                else { //no more days need to be filled, therefore, schedule is complete
                    schedule_validation = true;
                    return;
                }
            }
            else { //need workers that day
                //recurse through function again
                schedule_helper( avail, schedule , occurence , day , dailyNeed , maxShifts , schedule_validation );
                if ( schedule_validation ) { //if the recursion reveasl that the schedule is fully complete and validated, return
                    return;
                }
                else { //take out the worker
                    schedule[day].pop_back();
                    --(occurence[i]);
                }

            }
        }
    }
    

}

