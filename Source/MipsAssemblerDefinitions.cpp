#include "MipsAssemblerDefinitions.h"
#include "MIPSAssembler.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include "lexical_cast_ex.h"

using namespace boost;
using namespace std;

namespace MipsAssemblerDefinitions
{
    //RtRsImm Parser
    //-----------------------------
    struct RtRsImm
    {
        typedef void (CMIPSAssembler::*AssemblerFunctionType) (unsigned int, unsigned int, uint16);

        RtRsImm(AssemblerFunctionType Assembler) :
        m_Assembler(Assembler)
        {
            
        }

        void operator ()(tokenizer<>& Tokens, tokenizer<>::iterator& itToken, CMIPSAssembler* pAssembler)
        {
            unsigned int nRT, nRS;
            uint16 nImm;

            if(itToken == Tokens.end()) throw exception();
            nRT = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(itToken == Tokens.end()) throw exception();
            nRS = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(itToken == Tokens.end()) throw exception();
            nImm = lexical_cast_hex<string>((*(++itToken)).c_str());

            if(nRT == -1) throw exception();
            if(nRS == -1) throw exception();

            bind(m_Assembler, pAssembler, _1, _2, _3)(nRT, nRS, nImm);
        }

        AssemblerFunctionType m_Assembler;
    };

    //RdRsRt Parser
    //-----------------------------
    struct RdRsRt
    {
        typedef void (CMIPSAssembler::*AssemblerFunctionType) (unsigned int, unsigned int, unsigned int);

        RdRsRt(AssemblerFunctionType Assembler) :
        m_Assembler(Assembler)
        {
            
        }

        void operator ()(tokenizer<>& Tokens, tokenizer<>::iterator& itToken, CMIPSAssembler* pAssembler)
        {
            unsigned int nRT, nRS, nRD;

            if(itToken == Tokens.end()) throw exception();
            nRD = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(itToken == Tokens.end()) throw exception();
            nRS = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(itToken == Tokens.end()) throw exception();
            nRT = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(nRT == -1) throw exception();
            if(nRS == -1) throw exception();
            if(nRD == -1) throw exception();

            bind(m_Assembler, pAssembler, _1, _2, _3)(nRD, nRS, nRT);
        }

        AssemblerFunctionType m_Assembler;
    };

    //RtImm Parser
    //-----------------------------
    struct RtImm
    {
        typedef void (CMIPSAssembler::*AssemblerFunctionType) (unsigned int, uint16);

        RtImm(AssemblerFunctionType Assembler) :
        m_Assembler(Assembler)
        {
            
        }

        void operator ()(tokenizer<>& Tokens, tokenizer<>::iterator& itToken, CMIPSAssembler* pAssembler)
        {
            unsigned int nRT;
            uint16 nImm;

            if(itToken == Tokens.end()) throw exception();
            nRT = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(itToken == Tokens.end()) throw exception();
            nImm = lexical_cast_hex<string>((*(++itToken)).c_str());

            if(nRT == -1) throw exception();

            bind(m_Assembler, pAssembler, _1, _2)(nRT, nImm);
        }

        AssemblerFunctionType m_Assembler;
    };

    //RtRsSa Parser
    //-----------------------------
    struct RtRsSa
    {
        typedef void (CMIPSAssembler::*AssemblerFunctionType) (unsigned int, unsigned int, unsigned int);

        RtRsSa(AssemblerFunctionType Assembler) :
        m_Assembler(Assembler)
        {
            
        }

        void operator()(tokenizer<>& Tokens, tokenizer<>::iterator& itToken, CMIPSAssembler* pAssembler)
        {
            unsigned int nRT, nRS, nSA;

            if(itToken == Tokens.end()) throw exception();
            nRT = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(itToken == Tokens.end()) throw exception();
            nRS = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(itToken == Tokens.end()) throw exception();
            nSA = lexical_cast<unsigned int>((*(++itToken)).c_str());

            if(nRT == -1) throw exception();
            if(nRS == -1) throw exception();

            bind(m_Assembler, pAssembler, _1, _2, _3)(nRT, nRS, nSA);
        }

        AssemblerFunctionType m_Assembler;
    };

    //RtOfsBase Parser
    //-----------------------------
    struct RtOfsBase
    {
        typedef void (CMIPSAssembler::*AssemblerFunctionType) (unsigned int, uint16, unsigned int);

        RtOfsBase(AssemblerFunctionType Assembler) :
        m_Assembler(Assembler)
        {
            
        }

        void operator ()(tokenizer<>& Tokens, tokenizer<>::iterator& itToken, CMIPSAssembler* pAssembler)
        {
            unsigned int nRT, nBase;
            uint16 nOfs;

            if(itToken == Tokens.end()) throw exception();
            nRT = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(itToken == Tokens.end()) throw exception();
            nOfs = lexical_cast_hex<string>((*(++itToken)).c_str());

            if(itToken == Tokens.end()) throw exception();
            nBase = CMIPSAssembler::GetRegisterIndex((*(++itToken)).c_str());

            if(nBase == -1) throw exception();
            if(nRT == -1) throw exception();

            ((pAssembler)->*(m_Assembler))(nRT, nOfs, nBase);
        }

        AssemblerFunctionType m_Assembler;
    };

    template <typename Functor> 
    struct SpecInstruction : public Instruction
    {
        SpecInstruction(const char* sMnemonic, const Functor& Parser) :
        Instruction(sMnemonic),
        m_Parser(Parser)
        {
            
        }

        virtual void Invoke(tokenizer<>& Tokens, tokenizer<>::iterator& itToken, CMIPSAssembler* pAssembler)
        {
            m_Parser(Tokens, itToken, pAssembler);
        }

        Functor m_Parser;
    };

    //Instruction definitions
    //-------------------------------

    SpecInstruction<RdRsRt>     Instruction_ADDU    = SpecInstruction<RdRsRt>("ADDU", RdRsRt(&CMIPSAssembler::ADDU));
    SpecInstruction<RtRsImm>    Instruction_ADDIU   = SpecInstruction<RtRsImm>("ADDIU", RtRsImm(&CMIPSAssembler::ADDIU));
    SpecInstruction<RtOfsBase>  Instruction_LHU     = SpecInstruction<RtOfsBase>("LHU", RtOfsBase(&CMIPSAssembler::LHU));
    SpecInstruction<RtImm>      Instruction_LUI     = SpecInstruction<RtImm>("LUI", RtImm(&CMIPSAssembler::LUI));
    SpecInstruction<RtOfsBase>  Instruction_LW      = SpecInstruction<RtOfsBase>("LW", RtOfsBase(&CMIPSAssembler::LW));
    SpecInstruction<RdRsRt>     Instruction_MULT    = SpecInstruction<RdRsRt>("MULT", RdRsRt(&CMIPSAssembler::MULT));
    SpecInstruction<RdRsRt>     Instruction_OR      = SpecInstruction<RdRsRt>("OR", RdRsRt(&CMIPSAssembler::OR));
    SpecInstruction<RtRsSa>     Instruction_SLL     = SpecInstruction<RtRsSa>("SLL", RtRsSa(&CMIPSAssembler::SLL));
    SpecInstruction<RdRsRt>     Instruction_SLTU    = SpecInstruction<RdRsRt>("SLTU", RdRsRt(&CMIPSAssembler::SLTU));
    SpecInstruction<RtRsSa>     Instruction_SRA     = SpecInstruction<RtRsSa>("SRA", RtRsSa(&CMIPSAssembler::SRA));

    Instruction* g_Instructions[] = 
    {
        &Instruction_ADDU,
        &Instruction_ADDIU,
        &Instruction_LHU,
        &Instruction_LUI,
        &Instruction_LW,
        &Instruction_MULT,
        &Instruction_OR,
        &Instruction_SLL,
        &Instruction_SLTU,
        &Instruction_SRA,
        NULL,
    };

}

