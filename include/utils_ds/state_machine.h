#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdbool.h>

// typedef enum Sm_state_t Sm_state_t;
// typedef enum Sm_event_t Sm_event_t;
/*
    Special state which is a new state for a state with unacceptable event
    in the transition table
*/
enum {
    STATE_EVENT_IGNORED = -1
};

typedef enum {
    SM_EVENT_HANDLED,
    SM_EVENT_IGNORED,
    SM_ERR
} sm_result_t;

typedef enum {
    SM_ACTION_ERR = -1,
    SM_ACTION_ERR_NONE
} sm_action_result_t;

typedef struct State_machine State_machine;
typedef sm_action_result_t (*action_fn)(State_machine* sm, void* event_data);

typedef struct {
    int new_state;
    action_fn action; 
} Table_element;

struct State_machine {
    int current_state;
    bool is_event_received;
    int received_event;
    void* event_data;
    // const int** const transition_table;
    const Table_element* const * transition_table;
};


/*
    state-action tuple
*/
#define TABLE_ELEMENT(new_state, action) \
    {new_state, action}

/*
    __VA_ARGS__ is table element list 
    and the elements must be listed in the order of the event enum
*/
#define DEFINE_TRANSITION_TABLE_PER_STATE(table_name, ...) \
    static const Table_element table_name[] = {__VA_ARGS__}

/*
    This must be called after the DEFINE_TRANSITION_TABLE_PER_STATE()s.
    __VA_ARGS__ is a list of transition table per state and 
    the transition tables must be listed in the order of the state enum
*/
#define DEFINE_COMBINED_TRANSITION_TABLE(transition_table_name, ...) \
    static const Table_element* const transition_table_name[] = {__VA_ARGS__}\

// void sm_init(State_machine* sm, int initial_state, const Table_element** transition_table);
void sm_init(State_machine* sm, int initial_state, const Table_element* const transition_table[]);
sm_result_t sm_handle_event(State_machine* sm, int event_type, void* event_data);

/*
    This is to be called in an action function, if it is needed
*/
void sm_generate_internal_event(State_machine* sm, int event, void* event_data);




#endif