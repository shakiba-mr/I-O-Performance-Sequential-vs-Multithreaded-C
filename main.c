static void write_file(int id) {
    char path[256], buf[BUFFER_SIZE];
    snprintf(path, sizeof(path), TMP_DIR "/file_%04d.dat", id);
    FILE *f = fopen(path, "w");
    memset(buf, 'A' + (id % 26), sizeof(buf));
    int written = 0;
    while (written < FILE_SIZE) {
        fwrite(buf, 1, BUFFER_SIZE, f); written += BUFFER_SIZE;
    }
    fclose(f);
}
static void read_file(int id) {
    char path[256], buf[BUFFER_SIZE];
    snprintf(path, sizeof(path), TMP_DIR "/file_%04d.dat", id);
    FILE *f = fopen(path, "r");
    while (fread(buf, 1, BUFFER_SIZE, f) > 0) { /* consume */ }
    fclose(f);
}
/*sequential*/
static double run_sequential(void) {
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int i = 0; i < NUM_FILES; i++) write_file(i);
    for (int i = 0; i < NUM_FILES; i++) read_file(i);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    return (t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1e9;
}
/*multithreaded*/
typedef struct { int start, end; } Range;
static void *thread_worker(void *arg) {
    Range *r = (Range *)arg;
    for (int i = r->start; i < r->end; i++) write_file(i);
    for (int i = r->start; i < r->end; i++) read_file(i);
    return NULL;
}
static double run_multithreaded(void) {
    pthread_t threads[THREAD_COUNT];
    Range ranges[THREAD_COUNT];
    int chunk = NUM_FILES / THREAD_COUNT;
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (int t = 0; t < THREAD_COUNT; t++) {
        ranges[t].start = t*chunk;
        ranges[t].end   = (t==THREAD_COUNT-1)?NUM_FILES:(t+1)*chunk;
        pthread_create(&threads[t], NULL, thread_worker, &ranges[t]);
    }
    for (int t = 0; t < THREAD_COUNT; t++) pthread_join(threads[t], NULL);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    return (t1.tv_sec-t0.tv_sec)+(t1.tv_nsec-t0.tv_nsec)/1e9;
}
