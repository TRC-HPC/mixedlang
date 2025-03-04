#include "logger_f.h"

program demof
    use mixedlang
    use, intrinsic:: iso_fortran_env, only: stdout=>output_unit, stderr=>error_unit
    implicit none

    character(1024) :: log_msg = ""

    character(len=:), allocatable :: config
    integer :: ierr

    call get_config(config, ierr)
    if (ierr /= 0) stop ierr

    write(log_msg, *) "Config: ", config
    LOGGER("INFO", "demof", log_msg)

    deallocate (config)

end program demof
