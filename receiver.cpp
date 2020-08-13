#include <boost/program_options.hpp>
#include "KafkaConsumer.h"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    std::string brokers;
    std::string topic_name;
    std::string group_id;
    int partition = -1;
    int offset = -1;


    po::options_description options("Options");
    options.add_options()
        ("help,h",     "produce this help message")
        ("brokers,b",  po::value<std::string>(&brokers)->required(), 
                       "the kafka broker list")
        ("topic,t",    po::value<std::string>(&topic_name)->required(),
                       "the topic in which to write to")
        ("group-id,g", po::value<std::string>(&group_id)->required(),
                       "the consumer group id")
        ("partition,p", po::value<int>(&partition),
                       "the partition in which to read from")
        ("offset,o", po::value<int>(&partition),
                       "position in which to read from stream")
        ;

    po::variables_map vm;

    try {
        po::store(po::command_line_parser(argc, argv).options(options).run(), vm);
        po::notify(vm);
    }
    catch (std::exception& ex) {
        std::cout << "Error parsing options: " << ex.what() << std::endl;
        std::cout << std::endl;
        std::cout << options << std::endl;
        return 1;
    }

    KafkaConsumer consumer(brokers, group_id, topic_name, partition, offset);

     // Now read lines and write them into kafka
    while (true) 
    {
        // Try to consume a message
        std::string msg;
        if(consumer.GetMessage(msg))
        {
            std::cout << msg << std::endl;
        }
    }

    return 0;
}