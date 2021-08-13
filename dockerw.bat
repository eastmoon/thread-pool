@rem
@rem Copyright 2020 the original author jacky.eastmoon
@rem All commad module need 3 method :
@rem [command]        : Command script
@rem [command]-args   : Command script options setting function
@rem [command]-help   : Command description
@rem Basically, CLI will not use "--options" to execute function, "--help, -h" is an exception.
@rem But, if need exception, it will need to thinking is common or individual, and need to change BREADCRUMB variable in [command]-args function.
@rem NOTE, batch call [command]-args it could call correct one or call [command] and "-args" is parameter.
@rem

:: ------------------- batch setting -------------------
@rem setting batch file
@rem ref : https://www.tutorialspoint.com/batch_script/batch_script_if_else_statement.htm
@rem ref : https://poychang.github.io/note-batch/

@echo off
setlocal
setlocal enabledelayedexpansion

:: ------------------- declare CLI file variable -------------------
@rem retrieve project name
@rem Ref : https://www.robvanderwoude.com/ntfor.php
@rem Directory = %~dp0
@rem Object Name With Quotations=%0
@rem Object Name Without Quotes=%~0
@rem Bat File Drive = %~d0
@rem Full File Name = %~n0%~x0
@rem File Name Without Extension = %~n0
@rem File Extension = %~x0

set CLI_DIRECTORY=%~dp0
set CLI_FILE=%~n0%~x0
set CLI_FILENAME=%~n0
set CLI_FILEEXTENSION=%~x0

:: ------------------- declare CLI variable -------------------

set BREADCRUMB=cli
set COMMAND=
set COMMAND_BC_AGRS=
set COMMAND_AC_AGRS=

:: ------------------- declare variable -------------------

for %%a in ("%cd%") do (
    set PROJECT_NAME=%%~na
)
set PROJECT_ENV=dev
set PROJECT_SSH_USER=somesshuser
set PROJECT_SSH_PASS=somesshpass

:: ------------------- execute script -------------------

call :main %*
goto end

:: ------------------- declare function -------------------

:main (
    call :argv-parser %*
    call :%BREADCRUMB%-args %COMMAND_BC_AGRS%
    call :main-args %COMMAND_BC_AGRS%
    IF defined COMMAND (
        set BREADCRUMB=%BREADCRUMB%-%COMMAND%
        call :main %COMMAND_AC_AGRS%
    ) else (
        call :%BREADCRUMB%
    )
    goto end
)
:main-args (
    for %%p in (%*) do (
        if "%%p"=="-h" ( set BREADCRUMB=%BREADCRUMB%-help )
        if "%%p"=="--help" ( set BREADCRUMB=%BREADCRUMB%-help )
    )
    goto end
)
:argv-parser (
    set COMMAND=
    set COMMAND_BC_AGRS=
    set COMMAND_AC_AGRS=
    set is_find_cmd=
    for %%p in (%*) do (
        IF NOT defined is_find_cmd (
            echo %%p | findstr /r "\-" >nul 2>&1
            if errorlevel 1 (
                set COMMAND=%%p
                set is_find_cmd=TRUE
            ) else (
                set COMMAND_BC_AGRS=!COMMAND_BC_AGRS! %%p
            )
        ) else (
            set COMMAND_AC_AGRS=!COMMAND_AC_AGRS! %%p
        )
    )
    goto end
)

:: ------------------- Main mathod -------------------

:cli (
    goto cli-help
)

:cli-args (
    goto end
)

:cli-help (
    echo This is a Command Line Interface with project %PROJECT_NAME%
    echo If not input any command, at default will show HELP
    echo.
    echo Options:
    echo      --help, -h        Show more information with CLI.
    echo.
    echo Command:
    echo      dev               Develop mode.
    echo      build             Build application.
    echo      run               Run application.
    echo      test              Build and Run test application.
    echo.
    echo Run 'cli [COMMAND] --help' for more information on a command.
    goto end
)

:: ------------------- Command "dev" mathod -------------------
:cli-dev-prepare (
    echo ^> Initial cache
    @rem Initial cache
    IF NOT EXIST cache (
        mkdir cache
    )

    echo ^> Build image
    copy .\conf\docker\Dockerfile .
    docker build --rm^
        -t complier:%PROJECT_NAME%^
        .
    del Dockerfile
    goto end
)

:cli-dev-docker-run (
    docker rm -f complier-%PROJECT_NAME%
    docker run -ti --rm^
        -v %cd%/conf/cmake/release:/repo^
        -v %cd%/app/:/repo/app^
        -v %cd%/cache/release/build:/repo/build^
        -v %cd%/cache/release/publish:/repo/publish^
        --name complier-%PROJECT_NAME%^
        complier:%PROJECT_NAME% %*
    goto end
)

:cli-dev (
    call :cli-dev-prepare

    echo ^> Startup docker container instance
    call :cli-dev-docker-run

    goto end
)

:cli-dev-args (
    goto end
)

:cli-dev-help (
    echo Develop mode.
    echo.
    echo Options:
    echo.
    goto end
)

:: ------------------- Command "build" mathod -------------------
:cli-build (
    call :cli-dev-prepare

    echo ^> Startup docker container instance
    call :cli-dev-docker-run bash -l -c "source cli.sh build"

    goto end
)

:cli-build-args (
    goto end
)

:cli-build-help (
    echo Build application.
    echo.
    echo Options:
    echo.
    goto end
)

:: ------------------- Command "run" mathod -------------------
:cli-run (
    call :cli-dev-prepare

    echo ^> Startup docker container instance
    call :cli-dev-docker-run bash -l -c "source cli.sh run"

    goto end
)

:cli-run-args (
    goto end
)

:cli-run-help (
    echo Run application.
    echo.
    echo Options:
    echo.
    goto end
)

:: ------------------- Command "test" mathod -------------------

:cli-test-docker-run (
    docker rm -f complier-%PROJECT_NAME%
    docker run -ti --rm^
        -v %cd%/conf/cmake/test:/repo^
        -v %cd%/test/:/repo/test^
        -v %cd%/app/:/repo/test/app^
        -v %cd%/cache/test/build:/repo/build^
        -v %cd%/cache/test/publish:/repo/publish^
        --name complier-%PROJECT_NAME%^
        complier:%PROJECT_NAME% %*
    goto end
)

:cli-test (
    call :cli-dev-prepare

    echo ^> Startup docker container instance
    IF %TEST_MODE% == 0 (
        call :cli-test-docker-run
    )
    IF %TEST_MODE% == 1 (
        call :cli-test-docker-run bash -l -c "source cli.sh --log_level=all --log_format=CLF"
    )
    IF %TEST_MODE% == 2 (
        call :cli-test-docker-run bash -l -c "source cli.sh --log_level=warning --log_format=CLF"
    )
    IF %TEST_MODE% == 3 (
        call :cli-test-docker-run bash -l -c "source cli.sh --log_level=error --log_format=CLF"
    )
    goto end
)

:cli-test-args (
    set TEST_MODE=1
    for %%p in (%*) do (
        if "%%p"=="--dev" ( set TEST_MODE=0 )
        if "%%p"=="--all" ( set TEST_MODE=1 )
        if "%%p"=="--warn" ( set TEST_MODE=2 )
        if "%%p"=="--error" ( set TEST_MODE=3 )
    )
    goto end
)

:cli-test-help (
    echo Build and Run test application.
    echo Options only one last one will work.
    echo.
    echo Options:
    echo      --dev             Go into container.
    echo      --all             [DEFAULT] Show all test report.
    echo      --warn            Show test report with warn message.
    echo      --error           Show test report with error message.
    echo.
    goto end
)


:: ------------------- End method-------------------

:end (
    endlocal
)
