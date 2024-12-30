#define DLL_Class ProfilabDllWLED

#define INDEX_PRESETS 0

#include <profilab.hpp>

#include <http/EBHttpClient.hpp>
#include <json/EBJson.hpp>
#include <EBFile.hpp>

class DLL_CLASS : public Profilab
{
public: 
    DLL_CLASS() : lastState(nullptr)
    {
    }

    virtual void init()
    {
        addInput("$HOST");

        int presetCount = getConfigValue(INDEX_PRESETS);
        for( int i = 0; i < presetCount; i++ )
        {
            addInput(EBCpp::EBString("PRE ") + EBCpp::EBUtils::intToStr(i+1));
        }

        // Create output/input buffer
        if( lastState != nullptr ) delete[] lastState;
        lastState = new double[getInputCount()];
    };

    virtual void start()
    {
        for( int i = 0; i < getInputCount(); i++ )
        {
            lastState[i] = -1;
        }
    };

    virtual void update()
    {
        int inputCount = getInputCount()-1;

        if( client.isRequestFinished() )
        {
            for( int i = 0; i < inputCount; i++ )
            {
                double state = getInput(i+1)->getValue();
                if( state != lastState[i] )
                {
                    if( state >= PL_HIGH )
                    {
                        client.get(EBCpp::EBString("http://") + getInput(0)->getString() + ":80/win&PL=" + EBUtils::intToStr(i+1) );
                    }
                }
                lastState[i] = state;
            }
        }
    };

    virtual void stop()
    {

    };

    virtual void configure()
    {
        int32_t presetCount = getConfigValue(INDEX_PRESETS);

        EBCpp::EBFile file;
        file.setFileName("~config.tmp");
        file.open(EBCpp::EBFile::DIRECTION::WRITE_ONLY);
        file.write("Presets:\n");
        file.write(EBCpp::EBUtils::intToStr(presetCount) + "\n");

        file.close();

        system("notepad ~config.tmp");

        file.open(EBCpp::EBFile::DIRECTION::READ_ONLY);
        file.readLine();
        EBCpp::EBString in = file.readLine().trim();
        presetCount = in.toInt();
        file.close();

        EBCpp::EBFile::remove("~config.tmp");

        setConfigValue(INDEX_PRESETS, (double)presetCount);
    };

private:
    EBCpp::EBHttpClient<> client;

    double* lastState;
    EBCpp::EBString host;
};

#include <profilabDll.hpp>