#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/log.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <math.h>

#include <uORB/uORB.h>
#include <uORB/topics/gps_input.h>

extern "C" __EXPORT int gps_input_main(int argc, char **argv);

int gps_input_main(int argc, char **argv) {
    int gps_input_fd = orb_subscribe(ORB_ID(gps_input));
    orb_set_interval(gps_input_fd, 200); // limit the update rate to 200ms

    px4_pollfd_struct_t fds[] = {
        { .fd = gps_input_fd,   .events = POLLIN },
    };

    int error_counter = 0;

    // for (int i = 0; i < 10; i++) {
    while(true) {
        int poll_ret = px4_poll(fds, 1, 1000);

        if (poll_ret == 0) {
            PX4_ERR("Got no data within a second");
        } else if (poll_ret < 0) {
            if (error_counter < 10 || error_counter % 50 == 0) {
                PX4_ERR("ERROR return value from poll(): %d", poll_ret);
            }
            error_counter++;
        } else {
            if (fds[0].revents & POLLIN) {
                struct gps_input_s input;
                orb_copy(ORB_ID(gps_input), gps_input_fd, &input);
                PX4_INFO("Received gps_id : %d", input.gps_id);
                PX4_INFO("Received lat : %d", input.lat);
                PX4_INFO("Received lon : %d", input.lon);
                PX4_INFO("Received alt : %f", (double)input.alt);

                PX4_INFO("Received hdop : %f", (double)input.hdop);
                PX4_INFO("Received vdop : %f", (double)input.vdop);
                PX4_INFO("Received vn : %f", (double)input.vn);
                PX4_INFO("Received ve : %f", (double)input.ve);
                PX4_INFO("Received vd : %f", (double)input.vd);

                PX4_INFO("Received speed_accuracy : %f", (double)input.speed_accuracy);
                PX4_INFO("Received horiz_accuracy : %f", (double)input.horiz_accuracy);
                PX4_INFO("Received vert_accuracy : %f", (double)input.vert_accuracy);
                // rostopic pub -r 10 /mavros/gps_input/gpsinput_sub std_msgs/int 1
            }
        }
    }
    return 0;
}
