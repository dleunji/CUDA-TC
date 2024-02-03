#include <fstream>
#include <random>

template <typename ValueType>
void MakeDenseMatrix(int rows, int columns, ValueType *matrix,
                    std::default_random_engine generator)
{
    // std::uniform_real_distribution<float> distribution(-1.0, 1.0);

    for(int64_t i = 0;i < static_cast<int64_t>(rows) * columns; ++i) {
        // float temp = distribution(generator);
        float temp = 1;
        matrix[i] = ValueType(temp);
    }
}

template <typename InType, typename OutType, typename IndexType>
void calc(std::string benchmark, int k, bool valid)
{
    std::ifstream infile(benchmark, std::ifstream::in);
    std::string line;
    
    // read
    std::getline(infile, line, ',');
    const int m = std::stoi(line);
    std::getline(infile, line, ',');
    const int n = std::stoi(line);
    std::getline(infile, line, '\n');
    const int nonzeros = std::stoi(line);

    int *row_offsets = new int[m + 1];
    for(int i = 0;i < m + 1;i++) {
        if (i == m) std::getline(infile, line, '\n');
        else std::getline(infile, line, ' ');
        row_offsets[i] = std::stoi(line);
    }

    int *col_indices = new int[nonzeros];
    for(int i = 0;i < nonzeros;i++) {
        std::getline(infile, line, ' ');
        col_indices[i] = std::stoi(line);
    }

    std::default_random_engine generator;

    InType *values = new InType[nonzeros];
    InType *rhs_matrix = new InType[n * k];
    MakeDenseMatrix<InType>(n, k, rhs_matrix, generator);
    MakeDenseMatrix<InType>(1, nonzeros, values, generator);

    // Allocate the host output
    float *output_value_host = new float[m * k];


    if(valid) {
        // Initialize the output matrix with 0
        for(int i = 0;i < m * k;++i) {
            output_value_host[i] = 0.0f;
        }

        for(int i = 0;i < m;i++) {
            for(int j = row_offsets[i];j < row_offsets[i+1];j++) {
                int col_idx = col_indices[j];
                for(int l = 0;l < n;l++) {
                    output_value_host[i * k + l] += values[j] * rhs_matrix[col_idx * n + l];
                }
            }
        }

        for(int i = 0;i < m;i++) {
            for(int j = 0;j < n;j++) {
                printf("%1.f ", output_value_host[i * n + j]);
            }
            printf("\n");
        }
    }


}

int main()
{
    calc<float, float, int>("dlmc/test3.smtx", 16, true);


  

}