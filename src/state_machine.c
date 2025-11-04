#include <stddef.h>
#include <stdio.h>

#include "utils/state_machine.h"

// void sm_init(State_machine* sm, int initial_state, 
//                             const Table_element** transition_table) {
void sm_init(State_machine* sm, int initial_state, 
                            const Table_element* const transition_table[]) {
    sm->current_state = initial_state;
    sm->is_event_received = false;
    sm->received_event = 0;
    sm->event_data = NULL;
    sm->transition_table = transition_table;
}

sm_result_t sm_handle_event(State_machine* sm, int event_type, void* event_data) {
    
    int curr_state;
    int new_state;
    sm->received_event = event_type;
    sm->event_data = event_data;

    Table_element te;
    action_fn action;
    do {
        // reset is_event_received
        sm->is_event_received = false;
        
        curr_state = sm->current_state;
        te = sm->transition_table[curr_state][sm->received_event];
        new_state = te.new_state;
        if (new_state == STATE_EVENT_IGNORED) {
            // do nothing
            return SM_EVENT_IGNORED;
        }
        action = te.action;
        if (action != NULL) {
        // do action
            if (action(sm, sm->event_data) == SM_ACTION_ERR) {
                return SM_ERR;
            }
        }
        // do transition
        sm->current_state = new_state;
    } while(sm->is_event_received); // check internal event

    return SM_EVENT_HANDLED;
}

void sm_generate_internal_event(State_machine* sm, int event, void* event_data) {
    sm->is_event_received = true;
    sm->received_event = event;
    sm->event_data = event_data;
}
