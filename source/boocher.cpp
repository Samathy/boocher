#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#include <vector>
#include <sstream>

#include <mgclient.h>

class failed_query_exception: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Failed to connect to session";
    }
};

class session_connection_exception: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Failed to connect to session";
    }
};

class no_session_exception: public std::exception
{
    virtual const char* what() const throw()
    {
        return "No MG session open";
    }
};

class db
{
    public: 
        db()
        {
        }

        /** Constructor which initialises a new database
         */
        db(std::string hostname, std::string port)
        {
            this->hostname = hostname;
            this->port = port;

            this->new_session();
        }

        ~db()
        {
            mg_session_destroy(this->session);
        }

        /** Run query on the open session
         */
        void run(std::string query)
        {
            int status = 0;

            if(!this->session)
            {
                throw no_session_exception();
            }

            mg_session_run(this->session, query.c_str(), NULL, NULL);

            while((status = mg_session_pull(this->session, &this->results)) == 1)
            {
                this->rows++;
            }

        }
        
        int rows = 0;
        mg_result * results;

    private:
        mg_session_params * params = nullptr;
        mg_session * session = nullptr;

        std::string hostname;
        std::string port;

        void new_session()
        {
            std::cout << "Connecting to:" << this->hostname << ":" << this->port << std::endl;
            int status = 0;

            this->params = mg_session_params_make();

            //mg_session_params_set_host(this->params, this->hostname.c_str());
            //mg_session_params_set_port(this->params, std::atoi(this->port.c_str()));
            mg_session_params_set_host(this->params, "0.0.0.0");
            mg_session_params_set_port(this->params, 7687);
            mg_session_params_set_sslmode(params, MG_SSLMODE_REQUIRE);

            if (status = mg_connect(params, &this->session) < 0)
            {
                throw session_connection_exception();
            }
            
        }

};


bool run(char ** argc)
{
    auto mg = db(std::string(argc[2]), std::string(argc[3]));
    //std::string query = argc[4];
    mg.run("MATCH (n) RETURN n;");
    
    std::cout << "Rows: " << mg.rows << std::endl;

    return true;
}


/** Initialise the database.
 */
bool init(char ** argc)
{
    auto mg = db(std::string(argc[2]), std::string(argc[3]));

    std::stringstream query;

    query << "CREATE (n:library {name:\"boocherLibrary\"}) RETURN n;";

    std::cout << query.str() << std::endl;

    mg.run(query.str());

    std::cout << "Rows: " << mg.rows << std::endl;

    return true;
}

/** Adds a book
 */
bool add(char ** argc)
{
    auto mg = db(std::string(argc[2]), std::string(argc[3]));

    std::stringstream query;

    query << "MATCH (n:library) WHERE n.name= \"boocherLibrary\"; CREATE (b:book {title:\"" << argc[4] << "\", author:\"" << argc[5] << "\"})->[:IN]->(n) RETURN b;";

    std::cout << query.str() << std::endl;

    mg.run(query.str());

    std::cout << "Rows: " << mg.rows << std::endl;
    return true;
}

/** Sets a status on a book
 */
bool setstatus(char ** argc)
{
    auto mg = db(std::string(argc[2]), std::string(argc[3]));

    std::stringstream query;

    query << "MATCH (b:book) WHERE b.title = \"" << argc[4] <<"\"; CREATE (s:status {name: \"" <<  argc[5] << "\" }); CREATE (b)-[r:IS]->(s) RETURN b,r,s;";

    std::cout << query.str() << std::endl;

    mg.run(query.str());

    std::cout << "Rows: " << mg.rows << std::endl;

    return true;
}

/** Removes a book's status
 */
bool removestatus(char ** argc)
{
    return false;
}

/** Get a book and it's info
 */
bool get(char ** argc)
{
    return false;
}

/** Get all status on a book
 */
bool getall(char ** argc)
{
    return false;
}


int main(int argv, char ** argc)
{
    std::string help = 
    "\
    `boocher init [hostname] [port]` - Initialise the database \n\
    `boocher add  [title] [author]` - Add a book \n\
    `boocher set-status [title] [status]` - Sets a book's status ( e.g 'loaned', 'reading', 'finished' ). Books can have many statuses. \n\
    `boocher remove-status [title] [status]` - Removes a status from a book \n\
    `boocher get [title]` - Gets a book and it's assotiated statuses \n\
    `boocher get-all [status]` - Gets all books connected to given status.\
    ";

    if(argv <= 1)
    {
        std::cout << help;
        return 1;
    }

    std::string arg_one = argc[1];

    if(arg_one == "init")
    {
        if ( argv >= 3 )
            return init(argc);
    }
    else if(arg_one == "add")
    {
        if ( argv >= 4 )
            return add(argc);
    }
    else if(arg_one == "set-status")
    {
        return setstatus(argc);
    }
    else if(arg_one == "remove-status")
    {
        return removestatus(argc);
    }
    else if(arg_one == "get")
    {
        return get(argc);
    }
    else if(arg_one == "get-all")
    {
        return getall(argc);
    }
    else if(arg_one == "run")
    {
        return run(argc);
    }

    std::cout << "Unknown Options";
    std::cout << help;
    return 1;


}
