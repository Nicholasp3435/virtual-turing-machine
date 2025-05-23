Name = "Sample TM"                              /* Name of the Turing Machine */
Description = "Accepts bitstrings of length 2"  /* Describes the Turing Machine */

Q  = {qa, qr, q0, q1, q2}   /* Set of states */
S  = {0, 1}                 /* Set of input symbols */
G  = {0, 1, _}              /* Set of tape symbols */
q0 = q0                     /* Initial state */
F  = {qa}                   /* Set of accepting states */
b  = _                      /* The blank symbol */

/* Below is the definition of the transition function */
d(q0, 0) -> (q1, 0, R)
d(q0, 1) -> (q1, 1, R)
d(q0, _) -> (qr, _, R)
d(q1, 0) -> (q2, 0, R)
d(q1, 1) -> (q2, 1, R)
d(q1, _) -> (qr, _, R)
d(q2, 0) -> (qr, 0, R)
d(q2, 1) -> (qr, 1, R)
d(q2, _) -> (qa, _, R)