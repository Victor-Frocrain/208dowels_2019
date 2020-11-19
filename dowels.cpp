/*
** EPITECH PROJECT, 2020
** dowels
** File description:
** dowels.cpp
*/

#include "dowels.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>
#include <time.h>
#include <algorithm>
#include <tuple>

void dispHelp()
{
    std::ifstream fs("help");

    if (!fs.fail()) {
        std::cout << fs.rdbuf() << std::endl;
    }
}

bool parseNumber(const std::string &number, int &result)
{
    for (size_t i = 0; i < number.size(); i++) {
        if (number[i] < '0' || number[i] > '9') {
            return false;
        }
    }
    if (number.empty()) {
        return false;
    }
    result = std::stoi(number);
    return true;
}

bool fillDatas(Datas &datas, const int&ac, char **&args)
{
    std::string arg;
    int number = 0;

    for (int i = 1; i < ac; i++) {
        arg = args[i];
        if (parseNumber(arg, number)) {
            datas.inputs.push_back(number);
        }
        else {
            return false;
        }
    }
    return true;
}

void cleanString(std::string &ref)
{
    int nbNumbers = 0;
    std::string str = "";

    for (size_t i = 0; i < ref.length(); i++) {
        if (ref[i] == '-') {
            nbNumbers++;
        }
    }
    if (nbNumbers > 1) {
        str += ref[0];
        str += "-";
        str += ref[ref.length() -1];
        ref = str;
    }
}

std::string computeString(std::string ref, const int type, const int total = 8)
{
    std::string spaces = "";
    
    if (type == 0 && (ref.find("-8") != std::string::npos || ref == "8")) {
        ref = ref[0];
        ref += "+";
    }
    else {
        cleanString(ref);
    }
    for (int nbSpaces = total - ref.length(); nbSpaces > 0; nbSpaces--) {
        spaces += " ";
    }
    return ref + spaces;
}

double computeCoef(double firstValue, double secondValue)
{
    double coef = 1;
    double i = 1;
    double n = firstValue;
    double tmp = 1;
    double tmp2 = 1;
    double tmp3 = 1;
    double k2 = 0;
    double j = 0;

    if (secondValue < 0 || secondValue > firstValue) {
        coef = 0;
    }
    else {
        if (secondValue < (firstValue - secondValue)) {
            k2 = secondValue;
        }
        else {
            k2 = firstValue - secondValue;
        }
        j = 1;
        while (j <= k2) {
            tmp = n - i;
            tmp2 = tmp + 1;
            tmp3 = coef * tmp2;
            coef = tmp3 / i;
            i++;
            j++;
        }
    }
    return coef;
}

void computeBinomy(Datas &datas)
{
    int n = 100;

    for (int k = 0; k < 51; k++) {
        double coef(computeCoef(n, k));
        coef = (coef * (std::pow(datas.distrib, k)) * (std::pow((1 - datas.distrib), (n - k))));

        if (k < 8) {
            datas.coef.push_back(coef);
        }
        else if (k == 8) {
            datas.coef.push_back(coef);
        }
        else {
            datas.coef[datas.coef.size() - 1] += coef;
        }
    }
}

void computeDistrib(Datas &datas)
{
    datas.distrib = 0;
    for (size_t i = 0; i < datas.inputs.size(); i++) {
        datas.distrib += i * datas.inputs[i];
    }

    datas.distrib /= 10000;
}

void computeTab(Datas &datas, std::vector<std::string> &x, std::vector<int> &ox, std::vector<double> &tx)
{
    std::vector<std::string> restX;
    std::vector<int> restOx;
    std::vector<double> restTx;
    size_t i = 0;

    x.push_back("");
    ox.push_back(datas.inputs[0]);
    x[0] = "0";
    tx.push_back(datas.coef[0]);
    if (ox[0] >= 10) {
        x[0] = std::to_string(i);
    }
    else {
        for (i = 1; i < datas.inputs.size() && ox[0] < 10; i++) {
            ox[0] += datas.inputs[i];
            if (x[0] != "") {
                x[0] += "-";
            }
            x[0] += std::to_string(i);
            tx[0] += datas.coef[i];
        }
    }

    restX.push_back("");
    restOx.push_back(0);
    restTx.push_back(0);
    for (size_t n = datas.inputs.size() - 1; n > 0 && n >= i; n--) {
        if (restOx[restOx.size() - 1] >= 10) {
            std::reverse(restX[restX.size() - 1].begin(), restX[restX.size() - 1].end());
            restX.push_back("");
            restOx.push_back(0);
            restTx.push_back(0);
        }
        if (restX[restX.size() - 1] != "") {
            restX[restX.size() - 1] += "-";
        }
        restX[restX.size() - 1] += std::to_string(n);
        if (restOx.empty()) {
            restOx.push_back(0);
        }
        restOx[restOx.size() - 1] += datas.inputs[n];
        restTx[restTx.size() - 1] += datas.coef[n];
    }
    std::reverse(restX[restX.size() - 1].begin(), restX[restX.size() - 1].end());
    std::reverse(restX.begin(), restX.end());
    std::reverse(restOx.begin(), restOx.end());
    std::reverse(restTx.begin(), restTx.end());

    for (i = 0; i < restX.size() && i < restOx.size() && i < restTx.size(); i++) {
        x.push_back(restX[i]);
        ox.push_back(restOx[i]);
        tx.push_back(restTx[i]);
    }
}

void computeSquared(Datas &datas, const std::vector<std::string> &x, const std::vector<int> ox, const std::vector<double> tx)
{
    datas.square = 0;
    for (size_t i = 0; i < x.size() && i < ox.size() && i < tx.size(); i++) {
        if (x[i] != "" && tx[i] != 0) {
            datas.square += std::pow(ox[i] - (tx[i] * 100), 2) / (tx[i] * 100);
        }
    }
}

void computeDegrees(Datas &datas, const std::vector<std::string> &x)
{
    datas.degrees = 0;
    for (size_t i = 0; i < x.size(); i++) {
        if (x[i] != "") {
            datas.degrees += 1;
        }
    }
    datas.degrees -= 2;
}

void computeValidity(Datas &datas, double &min, double &max)
{
    int degree = datas.degrees;
    if (degree > 10) {
        return;
    }
    std::vector<double> percents = datas.tabValues.at(0);
    std::vector<double> values = datas.tabValues.at(degree);
    for (size_t i = 1; i < values.size() && i < percents.size(); i++) {
        if (datas.square < values[i]) {
                min =  percents[i];
            if (i > 0) {
                max =  percents[i - 1];
            }
            return;
        }
    }
    min = 1;
}

bool treatment(Datas &datas)
{
    std::vector<std::string> x;
    std::vector<int> ox;
    std::vector<double> tx;

    if (datas.inputs.size() != 9) {
        return false;
    }
    computeDistrib(datas);
    computeBinomy(datas);
    computeTab(datas, x, ox, tx);
    

    std::cout << "   x    |  ";
    for (size_t i = 0; i < x.size(); i++) {
        if (x[i] != "") {
            std::cout << computeString(x[i], 0) << "|  ";
        }
    }
    std::cout << "Total" << std::endl;

    std::cout << "  Ox    |  ";
    for (size_t i = 0; i < x.size(); i++) {
        if (x[i] != "") {
            std::cout << computeString(std::to_string(ox[i]), 1) << "|  ";
        }
    }
    std::cout << "100" << std::endl;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  Tx    |  ";
    for (size_t i = 0; i < tx.size(); i++) {
        if (x[i] != "") {
            std::cout << tx[i] * 100;
            if (tx[i] * 100 < 10) {
                std::cout << "     |  ";
            }
            else {
                std::cout << "    |  ";
            }
        }
    }
    std::cout << "100" << std::endl;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Distribution:                 B(100, " << datas.distrib << ")" << std::endl;

    computeSquared(datas, x, ox, tx);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Chi-squared:                  " << datas.square << std::endl;

    computeDegrees(datas, x);
    std::cout << std::fixed << std::setprecision(0);
    std::cout << "Degrees of freedom:           " << datas.degrees << std::endl;

    double min = 0;
    double max = 0;
    computeValidity(datas, min, max);
    std::cout << "Fit validity:                 ";
    if (min == 1) {
        std::cout << "P < " << min << "%" << std::endl;
    }
    else if (max == 99) {
        std::cout << "P > " << max << "%" << std::endl;
    }
    else {
        std::cout << min << "% < P < " << max << "%" << std::endl;
    }
    return true;
}

int sumAll(Datas &datas)
{
    int result = 0;

    for (size_t i = 0; i < datas.inputs.size(); i++) {
        result += datas.inputs[i];
    }
    return result;
}

int dowels(int ac, char **args)
{
    int result = MY_SUCCESS;
    Datas datas;

    if (ac >= 2) {
        std::string arg(args[1]);
        if (ac == 2 && arg == "-h") {
            dispHelp();
        }
        else if (ac == 10) {
            if (!fillDatas(datas, ac, args) || sumAll(datas) != 100) {
                return MY_ERROR;
            }
            else {
                if (!treatment(datas)) {
                    return MY_ERROR;
                }
            }
        }
        else {
            result = MY_ERROR;
        }
    }
    else
        result = MY_ERROR;
    return result;
}