#include <iostream>
#include <vector>
#include <algorithm>
#include<math.h>

using namespace std;

#define WeekDays 5
#define Exception "Not Found"
#define Course_Each_Week 2
#define null_number 0

struct possible_days{
    string date;
    vector<int> class_period={730, 930, 1130};
};
struct teachers{
    string name;
    int num_free_days;
    vector<possible_days> free_days;
    int num_of_courses;
    vector<string> course;
};
struct courses{
    string name;
    string day1;
    string day2;
    int start_time;
    int end_time;
};
struct day_schedule{
    vector<string> course;
    vector<string> teacher;
    vector<int> start_time;
    vector<int> end_time;
};
struct time_table{
    vector<int> class_period={730, 900, 930, 1100, 1130, 1300};
};
void init_teachers(teachers teacher_list[], int num_of_teachers){
    string token;
    for(int course_count=0; course_count < num_of_teachers; course_count++){
        cin>>teacher_list[course_count].name;
        cin>>teacher_list[course_count].num_free_days;
        possible_days day[teacher_list[course_count].num_free_days];
        for(int subject=0; subject<teacher_list[course_count].num_free_days; subject++){
            cin>>token;
            day[subject].date=token;
            teacher_list[course_count].free_days.push_back(day[subject]);
        }
        cin>>teacher_list[course_count].num_of_courses;
        for(int subject=0; subject<teacher_list[course_count].num_of_courses; subject++){
            cin>>token;
            teacher_list[course_count].course.push_back(token);
        }
    }
} 
bool compare_person(const teachers &person1, const teachers &person2){
    if(person2.num_free_days > person1.num_free_days)
        return 1;
    if(person2.num_free_days == person1.num_free_days)
        if(person2.name > person1.name)
            return 1;
    return 0;
}
bool compare_course(const courses &person1, const courses &person2){
    if(person2.name > person1.name)
            return 1;
    return 0;
}
void init_courses(courses course_list[], int num_semster_courses){
    for(int course_count=0; course_count < num_semster_courses; course_count++){
        cin>>course_list[course_count].name;
        cin>>course_list[course_count].day1;
        cin>>course_list[course_count].day2;
        int hour_start,hour_end,min_start,min_end;
        char dots; string time;
        cin>>hour_start>>dots>>hour_end;
        if(hour_end==0) time = to_string(hour_start) + to_string(hour_end) + "0";
        else time = to_string(hour_start) + to_string(hour_end);
        course_list[course_count].start_time=stoi(time);
        cin>>min_start>>dots>>min_end;
        if(min_end==0) time = to_string(min_start) + to_string(min_end) + "0";
        else time = to_string(min_start) + to_string(min_end);
        course_list[course_count].end_time=stoi(time);
    }
}
bool subset(vector<string> main_str, vector<string> subset_str){
    sort(main_str.begin(), main_str.end());
    sort(subset_str.begin(), subset_str.end());
    return includes(main_str.begin(), main_str.end(), subset_str.begin(), subset_str.end());
}
void correction(day_schedule schedule[], courses course_list[],  int count){
    if(schedule[0].course[count]!=schedule[1].course[count]){
        if(schedule[0].course.size()>schedule[1].course.size()){
            schedule[1].course.insert(schedule[1].course.begin()+count, course_list[count].name);
            schedule[1].teacher.insert(schedule[1].teacher.begin()+count, Exception);
            schedule[1].start_time.insert(schedule[1].start_time.begin()+count, null_number);
            schedule[1].end_time.insert(schedule[1].end_time.begin()+count, null_number);
        }
        else if(schedule[0].course.size()<schedule[1].course.size()){
            schedule[0].course.insert(schedule[0].course.begin()+count, course_list[count].name);
            schedule[0].teacher.insert(schedule[0].teacher.begin()+count, Exception);
            schedule[0].start_time.insert(schedule[0].start_time.begin()+count, null_number);
            schedule[0].end_time.insert(schedule[0].end_time.begin()+count, null_number);
        }
    }
}
void print_class(day_schedule schedule[],int count, int i){
    string start, end;
    if(schedule[i].teacher[count]==Exception) cout<<schedule[i].teacher[count]<<endl;
    else {
        cout<<schedule[i].teacher[count]<<": ";
        start=to_string(schedule[i].start_time[count]);
        end=to_string(schedule[i].end_time[count]);
        if(schedule[i].start_time[count]<1000) start="0"+start;
        if(schedule[i].end_time[count]<1000) end="0"+end;
        cout<<start.substr(0,2)<<":"<<start.substr(2,4)<<" ";
        cout<<end.substr(0,2)<<":"<<end.substr(2,4)<<endl;
    }
}
void print_schedule(day_schedule schedule[], courses course_list[], int num_semster_courses){
    int upper_bound=max(schedule[0].course.size(),schedule[1].course.size());
    for(int count=0; count<upper_bound; count++){
        if(schedule[0].course.size()>schedule[1].course.size()) cout<<schedule[0].course[count]<<endl;
        else cout<<schedule[1].course[count]<<endl;
        correction(schedule, course_list, count);
        for(int i=0; i<Course_Each_Week; i++){
            print_class(schedule,count, i);
        }
    }
}
int find_index(teachers teacher_list[], string target, int teacher_count){
    int day;
    for(int str=0; str<teacher_list[teacher_count].num_free_days; str++)
        if(teacher_list[teacher_count].free_days[str].date==target) {day = str; break;}
    return day;
}
void empty_struct(time_table days[Course_Each_Week][WeekDays],teachers teacher_list[], courses course_list[],
string target, int day, int period, int class_count,int course_count, int teacher_count){
    days[class_count][day].class_period[period]=null_number;
    days[class_count][day].class_period[period+1]=null_number;
    day = find_index(teacher_list, target, teacher_count);
    teacher_list[teacher_count].free_days[day].class_period[floor(period/2)]=null_number;
}
void schedule_day(day_schedule schedule[], teachers teacher_list[], courses course_list[], time_table days[Course_Each_Week][WeekDays],
vector<string> weekdays, int day, int period, int class_count, int course_count, int teacher_count){
    schedule[class_count].course.push_back(course_list[course_count].name);
    schedule[class_count].start_time.push_back(days[class_count][day].class_period[period]);
    schedule[class_count].end_time.push_back(days[class_count][day].class_period[period+1]);
    schedule[class_count].teacher.push_back(teacher_list[teacher_count].name);
    empty_struct(days,teacher_list,course_list,course_list[course_count].day1,day,period,class_count,course_count,teacher_count);
    for(int str=0; str<WeekDays; str++)
        if(weekdays[str]==course_list[course_count].day2) {day = str; break;}
    empty_struct(days,teacher_list,course_list,course_list[course_count].day2,day,period,class_count,course_count,teacher_count);
}
void register_teacher(day_schedule schedule[], teachers teacher_list[], courses course_list[],
time_table days[Course_Each_Week][WeekDays],vector<string> weekdays, int day, int period, int class_count,
int course_count,int num_of_teachers){
    for(int teacher_count=0; teacher_count<num_of_teachers; teacher_count++){
        for (string crs: teacher_list[teacher_count].course){
            if(course_list[course_count].name == crs){
            vector<string> temp = {course_list[course_count].day1, course_list[course_count].day2};
            vector<string> main;
            for(int count=0; count<teacher_list[teacher_count].num_free_days;count++){
                main.push_back(teacher_list[teacher_count].free_days[count].date);
            }
            if(subset(main, temp)){
                for(int count=0; count<teacher_list[teacher_count].num_free_days;count++)
                    if(teacher_list[teacher_count].free_days[count].date==weekdays[day])
                        for(int time: teacher_list[teacher_count].free_days[count].class_period)
                            if(time>=course_list[course_count].start_time && time==days[class_count][day].class_period[period]){
                                int day_temp = find_index(teacher_list, course_list[course_count].day2,teacher_count);
                                for(int time : teacher_list[teacher_count].free_days[day_temp].class_period)
                                    if(time>=course_list[course_count].start_time && 
                                       time==days[class_count][day].class_period[period]){
                                        schedule_day(schedule,teacher_list,course_list,days,weekdays,
                                        day,period,class_count,course_count,teacher_count);
                                        return;
                                    }
                            }
            }
            }
        }
    }
}
void register_course(day_schedule schedule[Course_Each_Week], teachers teacher_list[], courses course_list[],
time_table days[Course_Each_Week][WeekDays],vector<string> weekdays, int day, int period, int class_count,
int num_of_teachers, int num_semster_courses){
    bool en=0;
    for(int course_count=0; course_count<num_semster_courses; course_count++){
        for(string str: schedule[class_count].course)
            if(str.find(course_list[course_count].name)!=string::npos) en=1;
        if(en) {en=0;continue;}
        if(weekdays[day]==course_list[course_count].day1){
            int day_temp;
            for(int str=0; str<WeekDays; str++)
                if(weekdays[str]==course_list[course_count].day2) day_temp = str;
            if(days[class_count][day].class_period[period] >= course_list[course_count].start_time &&
                days[class_count][day_temp].class_period[period] >= course_list[course_count].start_time)
                if(days[class_count][day].class_period[period+1]<=course_list[course_count].end_time &&
                    days[class_count][day_temp].class_period[period] >= course_list[course_count].end_time)
                    {register_teacher(schedule,teacher_list,course_list,days,weekdays,day,period,class_count,course_count,
                    num_of_teachers); return;}
            else if(days[class_count][day].class_period[period+1] <= course_list[course_count].end_time &&
            days[class_count][day_temp].class_period[period+1] <= course_list[course_count].end_time)
                if(days[class_count][day].class_period[period]>=course_list[course_count].start_time && 
                days[class_count][day_temp].class_period[period]>=course_list[course_count].start_time)
                    {register_teacher(schedule,teacher_list,course_list,days,weekdays,day,period,class_count,course_count,
                    num_of_teachers); return;}
        }
    }
}
void simulate(day_schedule schedule[Course_Each_Week], teachers teacher_list[], courses course_list[],
time_table days[Course_Each_Week][WeekDays],vector<string> weekdays, int num_of_teachers, int num_semster_courses){
    for(int class_number=0; class_number<Course_Each_Week; class_number++){ // put in a void
        for(int day=0; day < WeekDays; day++){
            for(int period=0; period < WeekDays; period=period+2){
                register_course(schedule,teacher_list,course_list,days,weekdays,day,period,
                class_number,num_of_teachers, num_semster_courses);
            }
        }
    }
}
int main(){
    int num_of_teachers, num_semster_courses;
    cin>>num_of_teachers;
    teachers teacher_list[num_of_teachers];
    init_teachers(teacher_list, num_of_teachers);
    size_t len = sizeof(teacher_list)/sizeof(teacher_list[0]);
    sort(teacher_list, teacher_list+len, &compare_person);
    cin>>num_semster_courses;
    courses course_list[num_semster_courses];
    init_courses(course_list, num_semster_courses);
    size_t len1 = sizeof(course_list)/sizeof(course_list[0]);
    sort(course_list, course_list+len1, &compare_course);
    day_schedule schedule[Course_Each_Week];
    time_table days[Course_Each_Week][WeekDays];
    vector<string> weekdays={"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday"};
    simulate(schedule, teacher_list, course_list,days,weekdays,num_of_teachers, num_semster_courses);
    print_schedule(schedule,course_list, num_semster_courses);
}