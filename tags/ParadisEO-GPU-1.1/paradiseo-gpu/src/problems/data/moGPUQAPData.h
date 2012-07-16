/*
 <moGPUQAPData.h>
 Copyright (C) DOLPHIN Project-Team, INRIA Lille - Nord Europe, 2006-2012

 Karima Boufaras, Thé Van Luong

 This software is governed by the CeCILL license under French law and
 abiding by the rules of distribution of free software.  You can  use,
 modify and/ or redistribute the software under the terms of the CeCILL
 license as circulated by CEA, CNRS and INRIA at the following URL
 "http://www.cecill.info".

 As a counterpart to the access to the source code and  rights to copy,
 modify and redistribute granted by the license, users are provided only
 with a limited warranty  and the software's author,  the holder of the
 economic rights,  and the successive licensors  have only  limited liability.

 In this respect, the user's attention is drawn to the risks associated
 with loading,  using,  modifying and/or developing or reproducing the
 software by the user in light of its specific status of free software,
 that may mean  that it is complicated to manipulate,  and  that  also
 therefore means  that it is reserved for developers  and  experienced
 professionals having in-depth computer knowledge. Users are therefore
 encouraged to load and test the software's suitability as regards their
 requirements in conditions enabling the security of their systems and/or
 data to be ensured and,  more generally, to use and operate it in the
 same conditions as regards security.
 The fact that you are presently reading this means that you have had
 knowledge of the CeCILL license and that you accept its terms.

 ParadisEO WebSite : http://paradiseo.gforge.inria.fr
 Contact: paradiseo-help@lists.gforge.inria.fr
 */

#ifndef _moGPUQAPData_H_
#define _moGPUQAPData_H_

#include <memory/moGPUSpecificData.h>

template<class ElemType>
class moGPUQAPData: public moGPUSpecificData {

public:

	using moGPUSpecificData::sizeData;
	using moGPUSpecificData::GPUObject;

	/**
	 * Default Constructor
	 */

	moGPUQAPData() :
		moGPUSpecificData() {
	}

	/**
	 * Constructor
	 * @param _fileName the data file name
	 */

	moGPUQAPData(char* _fileName) {

		(*this).load(_fileName);

	}

	/**
	 * Constructor by copy
	 * @param _qapData the specific data of QAP
	 */

	moGPUQAPData(const moGPUQAPData & _qapData) {

		sizeData = _qapData.sizeData;
		a_h = new int[sizeData * sizeData];
		b_h = new int[sizeData * sizeData];
		for (int i = 0; i < sizeData; i++)

			for (int j = 0; j < sizeData; j++) {

				a_h[i * sizeData + j] = _qapData.a_h[i * sizeData + j];
				b_h[i * sizeData + j] = _qapData.b_h[i * sizeData + j];

			}

		GPUObject.memCopy(a_d, a_h, sizeData * sizeData);
		GPUObject.memCopy(b_d, b_h, sizeData * sizeData);

	}

	/**
	 * Assignement operator
	 * @param _qapData the specific data of QAP
	 * @return a QAP Data
	 */

	moGPUQAPData & operator=(const moGPUQAPData & _qapData) {

		sizeData = _qapData.sizeData;
		a_h = new int[sizeData * sizeData];
		b_h = new int[sizeData * sizeData];

		for (int i = 0; i < sizeData; i++)
			for (int j = 0; j < sizeData; j++) {

				a_h[i * sizeData + j] = _qapData.a_h[i * sizeData + j];
				b_h[i * sizeData + j] = _qapData.b_h[i * sizeData + j];

			}
		GPUObject.memCopy(a_d, a_h, sizeData * sizeData);
		GPUObject.memCopy(b_d, b_h, sizeData * sizeData);
		return (*this);
	}

	/*
	 * Destructor
	 */

	~moGPUQAPData() {
		GPUObject.memFree(a_d);
		GPUObject.memFree(b_d);
		delete[] a_h;
		delete[] b_h;
	}

	/*
	 *Load QAP data from file name
	 *@param _fileName the data file name to load
	 */

	void load(char* _fileName) {

		fstream file(_fileName, ios::in);
		if (!file) {

			string str = "QAPData: Could not open file [" + (string) _fileName
					+ "].";
			throw runtime_error(str);
		}

		unsigned i, j;
		file >> sizeData;
		a_h = new ElemType[sizeData * sizeData];
		b_h = new ElemType[sizeData * sizeData];

		for (i = 0; i < sizeData; i++)
			for (j = 0; j < sizeData; j++)
				file >> a_h[i * sizeData + j];
		for (i = 0; i < sizeData; i++)
			for (j = 0; j < sizeData; j++)
				file >> b_h[i * sizeData + j];

		//Allocate and copy QAP data from CPU memory to GPU global memory
		GPUObject.memCopy(a_d, a_h, sizeData * sizeData);
		GPUObject.memCopy(b_d, b_h, sizeData * sizeData);

	}

public:

	ElemType* a_h;
	ElemType* b_h;
	ElemType* a_d;
	ElemType* b_d;

};
#endif