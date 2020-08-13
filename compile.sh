gcc -std=c++11 \
-I$(pwd)/../ \
-I/usr/local/cppkafka/include \
-I/usr/local/librdkafka/include \
-L/usr/local/librdkafka/lib \
-L/usr/local/cppkafka/lib64 \
receiver.cpp -lm -lboost_system -lboost_program_options -lstdc++ -lcppkafka -lrdkafka -o SignalReceiver
