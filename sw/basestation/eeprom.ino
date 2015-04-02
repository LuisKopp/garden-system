
#define eeprom_job_address(recv, index) eeprom_jobs_start + eeprom_jobs_size * recv + sizeof(Interval) * index
#define eeprom_job_string_address(recv, index) eeprom_strings_start + 14 * (jobs_count * recv + index)

Interval get_recv_job(byte recv, byte index) {
    Interval job;

    EEPROM.get(eeprom_job_address(recv, index), job);

    return job;
}

void get_recv_job_string(byte recv, byte index, char* buff) {
    for (byte i = 0; i < 14; i++) {
        buff[i] = EEPROM.read(eeprom_job_string_address(recv, index) + i);

        //if (buff[i] == 0xFF) buff[i] = '0';
    }

    buff[13] = 0x00;
}

void save_recv_job(byte recv, byte index, Interval job) {
    EEPROM.put(eeprom_job_address(recv, index), job);

    // Generate string
    char job_name[14];

    sprintf_P(job_name,
        PSTR("%2d:%02d \x1a %2d:%02d"),
        job.start.s.h, job.start.s.m,
        job.end.s.h,   job.end.s.m
    );

    for (byte i = 0; i < 14; i++) {
        EEPROM.update(eeprom_job_string_address(recv, index) + i, job_name[i]);
    }
}

void save_settings() { EEPROM.put(eeprom_settings_start, settings); }
void load_settings() { EEPROM.get(eeprom_settings_start, settings); }

void format_eeprom() {
    Interval empty_job;

    memset(&empty_job, 0, sizeof(Interval));

    for (byte r = 0; r < RECEIVER_COUNT; r++) {
        for (byte j = 0; j < jobs_count; j++) {
            save_recv_job(r, j, empty_job);
        }
    }
}

void init_eeprom() {
    if (EEPROM.read(0) != 0xDC) {
        format_eeprom();

        EEPROM.write(0, 0xDC);
    }
}


Interval get_job(byte index)                { get_recv_job(state.selected_recv, index); }
void get_job_string(byte index, char* buff) { get_recv_job_string(state.selected_recv, index, buff); }
void save_job(byte index, Interval job)     { save_recv_job(state.selected_recv, index, job); }