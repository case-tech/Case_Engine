#include "PythonInspect.hpp"

#include "PythonCppCommon.hpp"
#include "PythonFunction.hpp"
#include "PythonInstance.hpp"
#include "PythonList.hpp"
#include "PythonMain.hpp"
#include "PythonModule.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PyCpp
        {
            constexpr char InspectModuleName[] = "inspect";

            Module GetInspectModule()
            {
                Python& py_ = Python::GetInstance();

                if (Module mod = py_.GetModule(InspectModuleName))
                    return mod;

                return py_.LoadModule(InspectModuleName);
            }

            Inspect::Inspect() : inspect_module_(GetInspectModule())
            {}

            Module Inspect::GetModule(PyObj data_a)
            {
                Function get_module_ = this->inspect_module_.GetFunction(
                        "getmodule");
                PyObj res_ = get_module_.CallV({Object(data_a)});
                Module mod_ = Module(res_);
                return mod_;
            }

            Module Inspect::GetModule(Function data_a)
            {
                return this->GetModule(data_a.function_);
            }
        } // namespace PyCpp
    } // namespace Aid
} // namespace CE_Kernel