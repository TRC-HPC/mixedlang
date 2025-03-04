! Fortran bindings for mixedlang lib.
module mixedlang
    implicit none
contains

    subroutine log_level(level)
        character(len=9), intent(out) :: level

        call log_level_fc(level)
    end subroutine

    subroutine set_level(level)
        character*(*), intent(in) :: level

        call set_level_fc(TRIM(ADJUSTL(level))//char(0))
    end subroutine

    subroutine set_headers(enable)
        logical, intent(in) :: enable

        call set_headers_fc(enable)
    end subroutine

    subroutine log(level, file, line, func, msg)
        character*(*), intent(in) :: level
        character*(*), intent(in) :: file
        integer, intent(in) :: line
        character*(*), intent(in) :: func
        character*(*), intent(in) :: msg

        call log_fc(TRIM(ADJUSTL(level))//char(0), &
                    TRIM(ADJUSTL(file))//char(0), &
                    line, &
                    TRIM(ADJUSTL(func))//char(0), &
                    TRIM(ADJUSTL(msg))//char(0))
    end subroutine

    subroutine get_config(config, ierr)
        character(len=:), allocatable, intent(out) :: config
        integer, intent(out) :: ierr

        integer :: length

        if (allocated(config)) deallocate (config)

        call get_config_fc(config, length, ierr)

        if (ierr == 0) then
            allocate (character(length) :: config)
            call get_config_fc(config, length, ierr)
            if (ierr /= 0) deallocate (config)
        end if

    end subroutine

end module
