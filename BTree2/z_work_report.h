#ifndef Z_WORK_REPORT_H
#define Z_WORK_REPORT_H

/*
    Features:
        -Not Implemented: Nothing was unimplemented


        -Implemented:      Everything required was implemented

        -Partly implemented: Removal

    Bugs     : Removal will not always be successful

    Reflections:           For BPTree my remove function does not always
                        properly function and will cause a few segmentation
                        errors as things progress. This is not a problem
                        I was able to remedy in my attempt to implement
                        the proper BPTree removal function.

                        I believe my removal was working properly and
                        completely in my first batch of tests, but as
                        I worked on my table I shifted my focus to that
                        and neglected removal.

                        Although it was not tested here, BPTree's
                        Iterators worked completely and perfectly. This
                        is displayed by the flawless functionality of my
                        table in my Data Base final project.

                        Insert and fix excess function properly.

                        Copy constructor, get, find, contains, etc work.

                        Removal was my only issue. I wish I gave myself time
                        to make it work proplery. At this stage I honestly
                        may as well have turned it in yesterday, but I had
                        hoped the extra day would be enough time to implement
                        it which was not the case.


*/


/*
testing insert
             4   5 Data= 2 Children= 0
             3 Data= 1 Children= 0
        3   4 Data= 2 Children= 3
             2 Data= 1 Children= 0
    2 Data= 1 Children= 2
             1 Data= 1 Children= 0
        1 Data= 1 Children= 2
             0 Data= 1 Children= 0


Testing remove
entry = 3
loose remove tree display
        4   5 Data= 2 Children= 0
      Data= 0 Children= 0
    3   4 Data= 2 Children= 3
        2 Data= 1 Children= 0

loose remove tree display
             5 Data= 1 Children= 0
             4 Data= 1 Children= 0
        4   5 Data= 2 Children= 3
             2 Data= 1 Children= 0
    2 Data= 1 Children= 2
             1 Data= 1 Children= 0
        1 Data= 1 Children= 2
             0 Data= 1 Children= 0

             5 Data= 1 Children= 0
             4 Data= 1 Children= 0
        4   5 Data= 2 Children= 3
             2 Data= 1 Children= 0
    2 Data= 1 Children= 2
             1 Data= 1 Children= 0
        1 Data= 1 Children= 2
             0 Data= 1 Children= 0

Press <RETURN> to close this window...
Press <RETURN> to close this window...
  */
#endif // Z_WORK_REPORT_H
