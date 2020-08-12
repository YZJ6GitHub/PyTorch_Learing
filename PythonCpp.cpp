#pragma once
#include<iostream>
#include <python.h>
#include<vector>
#include <stdlib.h>  
#include <stdio.h>
using namespace std;


bool RunPythonStatement()
{
    //初始化python解释器
    Py_Initialize();  
    if (!Py_IsInitialized())
    {
        return false;
    }
    try
    {
        //导入python的sys库
        PyRun_SimpleString("import sys\n");
        //输入字符串
        PyRun_SimpleString("sys.stdout.write('hello,hello!\\n')\n");
        //输出运算
        PyRun_SimpleString("sys.stdout.write('3+4={}\\n'.format(3+4))\n");

    }
    catch (const std::exception&)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;

    }
    Py_Finalize();
}
//调用没有参数的Func
bool InvokeNonParasFunByAPI(string spath, string smodule, string sFunc)
{
    //初始化python解释器
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        return false;
    }
    try
    {
        //导入python的sys库
        PyRun_SimpleString("import sys\n");
        PyRun_SimpleString("import cv2\n");
        //将脚本文件加载到系统路径下
        string sImportDir = "sys.path.append('" + spath + "')\n";
        PyRun_SimpleString(sImportDir.c_str());

        //加载模块
        PyObject* moduleName = PyUnicode_FromString(smodule.c_str());
        PyObject* pmodule = PyImport_Import(moduleName);
        if (!pmodule)
        {
            cout << "Import module failed!" << endl;
            return false;
        }
        PyObject* pFunc = PyObject_GetAttrString(pmodule, sFunc.c_str());

        if (!pFunc || !PyCallable_Check(pFunc))
        {
            cout << "Get Function Failed!" << endl;
            return false;
        }
        PyObject* pResult = PyObject_CallObject(pFunc, nullptr);
        if (!pResult)
        {
            cout << "Get pResult Failed!" << endl;
            return false;
        }
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }
    Py_Finalize();
    return true;
}
//调用有参数的Func

bool InvokeParasFunByAPI(string spath, string smodule, string sFunc, vector<int>paras)
{
    //初始化python解释器
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        return false;
    }
    try
    {
        //导入python的sys库
        PyRun_SimpleString("import sys\n");
        //将脚本文件加载到系统路径下
        string sImportDir = "sys.path.append('" + spath + "')\n";
        PyRun_SimpleString(sImportDir.c_str());

        //加载模块
        PyObject* moduleName = PyUnicode_FromString(smodule.c_str());
        PyObject* pmodule = PyImport_Import(moduleName);
        if (!pmodule)
        {
            cout << "Import module failed!" << endl;
            return false;
        }
        // PyObject_GetAttrString 获取对象值:加载模块中的方法
        PyObject* pFunc = PyObject_GetAttrString(pmodule, sFunc.c_str());
        if (!pFunc || !PyCallable_Check(pFunc))
        {
            cout << "Get Function Failed!" << endl;
            return false;
        }
        PyObject* pResult = nullptr;

        //建立入参的数据结构
        //PyTuple_SetItem ： 添加元素
        PyObject* args = PyTuple_New((int)paras.size());
        for (int idx = 0; idx < paras.size(); idx++)
            PyTuple_SetItem(args, idx, PyLong_FromLong(paras[idx]));
        //调用函数
        pResult = PyObject_CallObject(pFunc, args);
        if (!pResult)
        {
            cout << "Get pResult Failed!" << endl;
            return false;
        }

        cout << "result = " << PyLong_AsLong(pResult) << endl;
    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }
    Py_Finalize();
    return true;
}

int main()
{
   
    vector<int>papas;
    papas.push_back(10);
    papas.push_back(30);
    string sCurrnetDir = "C:/Users/Yuzhe/Desktop/JniGetFunction/x64/Debug/pythonScript";

    InvokeNonParasFunByAPI(sCurrnetDir,"helloWorld","PrintHelloWorld");

    InvokeParasFunByAPI(sCurrnetDir, "GetSum", "GetSumValue", papas);
    return 0;
}