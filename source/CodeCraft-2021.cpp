#include "Config.h"
#include "CloudServerCenter.h"

#ifdef TEST
// 训练数据文件
const char *TestFile = "../training-data/training-1.txt";
// 输出文件
const char *OutFile = "../output.txt";
#endif

int main() {
#ifdef TEST
    cout << "Input from \"" << TestFile << "\"" << endl;
    cout << "Writing to \"" << OutFile << "\" ..." << endl;
    cout << "Auto running, don't exit, please wait ..." << endl;
    ifstream fin{TestFile};
    ofstream fout{OutFile};
    assert(fin.is_open());
    assert(fout.is_open());
    // 重定向输入，并保存原输入流缓冲区指针
    streambuf *oldCin = cin.rdbuf(fin.rdbuf());
    // 重定向输出，并保存原输出流缓冲区指针
    streambuf *oldCout = cout.rdbuf(fout.rdbuf());
    // 重定向stderr
    freopen("../log.txt","w",stderr);
#endif

    CloudServerCenter center;
    center.autoGenerate();

#ifdef TEST
    cin.rdbuf(oldCin);   // 恢复标准输入
    cout.rdbuf(oldCout); // 恢复标准输出
    fin.close();         // 关闭输入文件
    fout.close();        // 关闭输出文件
    // 输出程序时间
    cout << "Program Run Time: " << (double) clock() / CLOCKS_PER_SEC << "s" << endl;
#endif
    return 0;
}
