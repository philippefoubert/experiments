/*============================================================================*/
/* File Description                                                           */
/*============================================================================*/
/**
 * @file        ocl_log.hpp
 * @brief       OpenCL utilities
 */
/* author       P. FOUBERT                                                    */
/*============================================================================*/
#ifndef OCL_LOG_HPP_
#define OCL_LOG_HPP_

/*============================================================================*/
/* Forward declarations                                                       */
/*============================================================================*/
namespace cl
{
    class Device;
    class Error;
}

/*============================================================================*/
/* Macros                                                                     */
/*============================================================================*/
#define LOG_CL_ERROR(err, msg) \
    logCLError(err, __FILE__, __LINE__, msg)

/*============================================================================*/
/* Functions Prototype                                                        */
/*============================================================================*/
void logCLDeviceInfo(const cl::Device &i_device);

void logCLError(const cl::Error &i_err, const char *i_file, int i_line, const char *i_msg);
void logCLError(int              i_err, const char *i_file, int i_line, const char *i_msg);

#endif //OCL_LOG_HPP_
