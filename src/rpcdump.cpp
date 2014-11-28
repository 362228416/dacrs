// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "base58.h"
#include "rpcserver.h"
#include "init.h"
#include "main.h"
#include "sync.h"
#include "wallet.h"

#include <fstream>
#include <stdint.h>

#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "json/json_spirit_value.h"
#include "json/json_spirit_writer_template.h"



using namespace json_spirit;
using namespace std;

void EnsureWalletIsUnlocked();

string static EncodeDumpTime(int64_t nTime) {
    return DateTimeStrFormat("%Y-%m-%dT%H:%M:%SZ", nTime);
}

int64_t static DecodeDumpTime(const string &str) {
    static const boost::posix_time::ptime epoch = boost::posix_time::from_time_t(0);
    static const locale loc(locale::classic(),
        new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%SZ"));
    istringstream iss(str);
    iss.imbue(loc);
    boost::posix_time::ptime ptime(boost::date_time::not_a_date_time);
    iss >> ptime;
    if (ptime.is_not_a_date_time())
        return 0;
    return (ptime - epoch).total_seconds();
}

string static EncodeDumpString(const string &str) {
    stringstream ret;
    for (auto c : str) {
        if (c <= 32 || c >= 128 || c == '%') {
            ret << '%' << HexStr(&c, &c + 1);
        } else {
            ret << c;
        }
    }
    return ret.str();
}

string DecodeDumpString(const string &str) {
    stringstream ret;
    for (unsigned int pos = 0; pos < str.length(); pos++) {
        unsigned char c = str[pos];
        if (c == '%' && pos+2 < str.length()) {
            c = (((str[pos+1]>>6)*9+((str[pos+1]-'0')&15)) << 4) | 
                ((str[pos+2]>>6)*9+((str[pos+2]-'0')&15));
            pos += 2;
        }
        ret << c;
    }
    return ret.str();
}

Value importprivkey(const Array& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 3)
        throw runtime_error(
            "importprivkey \"bitcoinprivkey\" ( \"label\" rescan )\n"
            "\nAdds a private key (as returned by dumpprivkey) to your wallet.\n"
            "\nArguments:\n"
            "1. \"bitcoinprivkey\"   (string, required) The private key (see dumpprivkey)\n"
            "2. \"label\"            (string, optional) an optional label\n"
            "3. rescan               (boolean, optional, default=true) Rescan the wallet for transactions\n"
            "\nExamples:\n"
            "\nDump a private key\n"
            + HelpExampleCli("dumpprivkey", "\"myaddress\"") +
            "\nImport the private key\n"
            + HelpExampleCli("importprivkey", "\"mykey\"") +
            "\nImport using a label\n"
            + HelpExampleCli("importprivkey", "\"mykey\" \"testing\" false") +
            "\nAs a json rpc call\n"
            + HelpExampleRpc("importprivkey", "\"mykey\", \"testing\", false")
        );

    EnsureWalletIsUnlocked();

    string strSecret = params[0].get_str();
    string strLabel = "";
    if (params.size() > 1)
        strLabel = params[1].get_str();

    // Whether to perform rescan after import
    bool fRescan = true;
    if (params.size() > 2)
        fRescan = params[2].get_bool();

    CBitcoinSecret vchSecret;
    bool fGood = vchSecret.SetString(strSecret);

    if (!fGood) throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid private key encoding");

    CKey key = vchSecret.GetKey();
    if (!key.IsValid()) throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Private key outside allowed range");

    CPubKey pubkey = key.GetPubKey();
    CKeyID vchAddress = pubkey.GetKeyID();
    {
        LOCK2(cs_main, pwalletMain->cs_wallet);

//        pwalletMain->MarkDirty();


        // Don't throw error in case a key is already there
        if (pwalletMain->count(vchAddress))
        	throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Private key is in the wallet");;

//        pwalletMain->mapKeyMetadata[vchAddress].nCreateTime = 1;

        if (!pwalletMain->AddKey(key))
            throw JSONRPCError(RPC_WALLET_ERROR, "Error adding key to wallet");

        // whenever a key is imported, we need to scan the whole chain
//        pwalletMain->nTimeFirstKey = 1; // 0 would be considered 'no value'

        pwalletMain->SynchronizRegId(vchAddress,*pAccountViewTip);
        if (fRescan) {
            pwalletMain->ScanForWalletTransactions(chainActive.Genesis(), true);
        }
    }

    return "OK";
}

Value importwallet(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw runtime_error(
            "importwallet \"filename\"\n"
            "\nImports keys from a wallet dump file (see dumpwallet).\n"
            "\nArguments:\n"
            "1. \"filename\"    (string, required) The wallet file\n"
            "\nExamples:\n"
            "\nDump the wallet\n"
            + HelpExampleCli("dumpwallet", "\"test\"") +
            "\nImport the wallet\n"
            + HelpExampleCli("importwallet", "\"test\"") +
            "\nImport using the json rpc call\n"
            + HelpExampleRpc("importwallet", "\"test\"")
        );

    throw JSONRPCError(RPC_INVALID_PARAMETER, "todo work list");

    ///todo work list
//    EnsureWalletIsUnlocked();
//
//    ifstream file;
//    file.open(params[0].get_str().c_str(), ios::in | ios::ate);
//    if (!file.is_open())
//        throw JSONRPCError(RPC_INVALID_PARAMETER, "Cannot open wallet dump file");
//
//    int64_t nTimeBegin = chainActive.Tip()->nTime;
//
//    bool fGood = true;
//
//    int64_t nFilesize = max((int64_t)1, (int64_t)file.tellg());
//    file.seekg(0, file.beg);
//
//    pwalletMain->ShowProgress(_("Importing..."), 0); // show progress dialog in GUI
//    while (file.good()) {
//        pwalletMain->ShowProgress("", max(1, min(99, (int)(((double)file.tellg() / (double)nFilesize) * 100))));
//        string line;
//        getline(file, line);
//        if (line.empty() || line[0] == '#')
//            continue;
//
//        vector<string> vstr;
//        boost::split(vstr, line, boost::is_any_of(" "));
//        if (vstr.size() < 2)
//            continue;
//        CBitcoinSecret vchSecret;
//        if (!vchSecret.SetString(vstr[0]))
//            continue;
//        CKey key = vchSecret.GetKey();
//        CPubKey pubkey = key.GetPubKey();
//        CKeyID keyid = pubkey.GetID();
////        if (pwalletMain->HaveKey(keyid)) {
////            LogPrint("INFO","Skipping import of %s (key already present)\n", CBitcoinAddress(keyid).ToString());
////            continue;
////        }
//        assert(0);
//        int64_t nTime = DecodeDumpTime(vstr[1]);
//        string strLabel;
//        bool fLabel = true;
//        for (unsigned int nStr = 2; nStr < vstr.size(); nStr++) {
//            if (boost::algorithm::starts_with(vstr[nStr], "#"))
//                break;
//            if (vstr[nStr] == "change=1")
//                fLabel = false;
//            if (vstr[nStr] == "reserve=1")
//                fLabel = false;
//            if (boost::algorithm::starts_with(vstr[nStr], "label=")) {
//                strLabel = DecodeDumpString(vstr[nStr].substr(6));
//                fLabel = true;
//            }
//        }
//        LogPrint("INFO","Importing %s...\n", CBitcoinAddress(keyid).ToString());
//        if (!pwalletMain->AddKey(key)) {
//            fGood = false;
//            continue;
//        }
////        pwalletMain->mapKeyMetadata[keyid].nCreateTime = nTime;
////        if (fLabel)
////            pwalletMain->SetAddressBook(keyid, strLabel, "receive");
////        nTimeBegin = min(nTimeBegin, nTime);
//    }
//    file.close();
//    pwalletMain->ShowProgress("", 100); // hide progress dialog in GUI
//
//    CBlockIndex *pindex = chainActive.Tip();
//    while (pindex && pindex->pprev && pindex->nTime > nTimeBegin - 7200)
//        pindex = pindex->pprev;
//
////    if (!pwalletMain->nTimeFirstKey || nTimeBegin < pwalletMain->nTimeFirstKey)
////        pwalletMain->nTimeFirstKey = nTimeBegin;
//
//    LogPrint("INFO","Rescanning last %i blocks\n", chainActive.Height() - pindex->nHeight + 1);
//    pwalletMain->ScanForWalletTransactions(pindex);
//
////    pwalletMain->MarkDirty();
//
//    if (!fGood)
//        throw JSONRPCError(RPC_WALLET_ERROR, "Error adding some keys to wallet");
//
//    return Value::null;
}

Value dumpprivkey(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw runtime_error(
            "dumpprivkey \"soypayaddress\"\n"
            "\nReveals the private key corresponding to 'soypayaddress'.\n"
            "Then the importprivkey can be used with this output\n"
            "\nArguments:\n"
            "1. \"soypayaddress\"   (string, required) The soypay address for the private key\n"
            "\nResult:\n"
            "\"key\"                (string) The private key\n"
            "\nExamples:\n"
            + HelpExampleCli("dumpprivkey", "\"myaddress\"")
            + HelpExampleCli("importprivkey", "\"mykey\"")
            + HelpExampleRpc("dumpprivkey", "\"myaddress\"")
        );

    EnsureWalletIsUnlocked();

    string strAddress = params[0].get_str();
    CBitcoinAddress address;
    if (!address.SetString(strAddress))
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid Bitcoin address");
    CKeyID keyID;
    if (!address.GetKeyID(keyID))
        throw JSONRPCError(RPC_TYPE_ERROR, "Address does not refer to a key");
    CKey vchSecret;
    if (!pwalletMain->GetKey(keyID, vchSecret))
        throw JSONRPCError(RPC_WALLET_ERROR, "Private key for address " + strAddress + " is not known");
    return CBitcoinSecret(vchSecret).ToString();
}



Value dumpwallet(const Array& params, bool fHelp) {
	if (fHelp || params.size() != 1)
		throw runtime_error("dumpwallet \"filename\"\n"
				"\nDumps all wallet keys in a human-readable format.\n"
				"\nArguments:\n"
				"1. \"filename\"    (string, required) The filename\n"
				"\nExamples:\n" + HelpExampleCli("dumpwallet", "\"test\"") + HelpExampleRpc("dumpwallet", "\"test\""));

	EnsureWalletIsUnlocked();

	ofstream file;
	file.open(params[0].get_str().c_str());
	if (!file.is_open())
		throw JSONRPCError(RPC_INVALID_PARAMETER, "Cannot open wallet dump file");

	Object reply;
	reply.push_back(Pair("created by Soypay", CLIENT_BUILD + CLIENT_DATE));
	reply.push_back(Pair("Created Time ", EncodeDumpTime(GetTime())));
	reply.push_back(Pair("Best block index hight ", chainActive.Height()));
	reply.push_back(Pair("Best block hash ", chainActive.Tip()->GetBlockHash().ToString()));

	map<CKeyID, CKeyStoreValue> tepmKeyPool = pwalletMain->GetKeyPool();
	int index = 0;
	for (auto &te : tepmKeyPool) {
		reply.push_back(Pair(strprintf("index%d",index++), te.second.ToString()));
	}
	file <<  write_string(Value(reply), true);

	file.close();
	return "dump OK";
}
