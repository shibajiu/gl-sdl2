#pragma once
#include <fstream>
#include <io.h>
#include <string>
#include <vector>

class DICOMReader {
private:
	const DICOMReader& operator =(const DICOMReader& o) = delete;
	DICOMReader(const DICOMReader&) = delete;
	const DICOMReader& operator =(const DICOMReader&& o) = delete;
	DICOMReader(const DICOMReader&&) = delete;

	void getAllFiles(string path, vector<string>& files);

public:
	DICOMReader();
	~DICOMReader();
};

