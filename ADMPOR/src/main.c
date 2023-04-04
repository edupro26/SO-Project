



int main(int argc, char *argv[])
{
    // init data structures
    struct main_data *data = create_dynamic_memory(sizeof(struct main_data));
    struct comm_buffers *buffers = create_dynamic_memory(sizeof(struct comm_buffers));
    buffers->main_client = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    buffers->client_interm = create_dynamic_memory(sizeof(struct circular_buffer));
    buffers->interm_enterp = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    // execute main code
    main_args(argc, argv, data);
    create_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, buffers);
    launch_processes(buffers, data);
    user_interaction(buffers, data);
    // release memory before terminating
    destroy_dynamic_memory(data);
    destroy_dynamic_memory(buffers->main_client);
    destroy_dynamic_memory(buffers->client_interm);
    destroy_dynamic_memory(buffers->interm_enterp);
    destroy_dynamic_memory(buffers);
}

void create_dynamic_memory_buffers(struct main_data* data) {
    data->client_pids = create_dynamic_memory(data->n_clients * sizeof(int));
    data->intermediary_pids = create_dynamic_memory(data->n_intermediaries * sizeof(int));
    data->enterprise_pids = create_dynamic_memory(data->n_enterprises * sizeof(int));
    
    data->client_stats = create_dynamic_memory(data->n_clients * sizeof(int));
    data->intermediary_stats = create_dynamic_memory(data->n_intermediaries * sizeof(int));
    data->enterprise_stats = create_dynamic_memory(data->n_enterprises * sizeof(int));
}

void create_shared_memory_buffers(struct main_data* data, struct comm_buffers* buffers) {
    // Create shared memory for main to client buffer
    buffers->main_to_client.buffer = (struct operation*) create_shared_memory("main_to_client", data->buffers_size * sizeof(struct operation));
    buffers->main_to_client.ptrs = (int*) create_shared_memory("main_to_client_ptrs", 2 * sizeof(int));
    memset(buffers->main_to_client.ptrs, 0, 2 * sizeof(int));

    // Create shared memory for client to intermediary buffers
    for (int i = 0; i < data->n_clients; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "client_to_interm_%d", i);
        sprintf(ptrs_name, "client_to_interm_%d_ptrs", i);
        buffers->client_to_interm[i].buffer = (struct operation*) create_shared_memory(buffer_name, data->buffers_size * sizeof(struct operation));
        buffers->client_to_interm[i].ptrs = (struct ptrs*) create_shared_memory(ptrs_name, sizeof(struct ptrs));
        memset(buffers->client_to_interm[i].ptrs, 0, sizeof(struct ptrs));
    }

    // Create shared memory for intermediary to enterprise buffers
    for (int i = 0; i < data->n_intermediaries; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "interm_to_enterp_%d", i);
        sprintf(ptrs_name, "interm_to_enterp_%d_ptrs", i);
        buffers->interm_to_enterp[i].buffer = (struct operation*) create_shared_memory(buffer_name, data->buffers_size * sizeof(struct operation));
        buffers->interm_to_enterp[i].ptrs = (struct ptrs*) create_shared_memory(ptrs_name, sizeof(struct ptrs));
        memset(buffers->interm_to_enterp[i].ptrs, 0, sizeof(struct ptrs));
    }

    // Create shared memory for enterprise to intermediary buffers
    for (int i = 0; i < data->n_enterprises; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "enterp_to_interm_%d", i);
        sprintf(ptrs_name, "enterp_to_interm_%d_ptrs", i);
        buffers->enterp_to_interm[i].buffer = (struct operation*) create_shared_memory(buffer_name, data->buffers_size * sizeof(struct operation));
        buffers->enterp_to_interm[i].ptrs = (struct ptrs*) create_shared_memory(ptrs_name, sizeof(struct ptrs));
        memset(buffers->enterp_to_interm[i].ptrs, 0, sizeof(struct ptrs));
    }

    // Create shared memory for intermediary to client buffers
    for (int i = 0; i < data->n_intermediaries; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "interm_to_client_%d", i);
        sprintf(ptrs_name, "interm_to_client_%d_ptrs", i);
        buffers->interm_to_client[i].buffer = (struct operation*) create_shared_memory(buffer_name, data->buffers_size * sizeof(struct operation));
        buffers->interm_to_client[i].ptrs = (struct ptrs*) create_shared_memory(ptrs_name, sizeof(struct ptrs));
        memset(buffers->interm
    }
}