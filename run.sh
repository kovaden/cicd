#!/bin/bash

(seq 0 999 | nc localhost 9000) &
(seq 1000 1999 | nc localhost 9000) &
