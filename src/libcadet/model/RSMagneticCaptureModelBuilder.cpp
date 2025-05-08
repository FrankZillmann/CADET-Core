#include "model/RSMagneticCaptureModel.hpp"
#include "cadet/ParameterProvider.hpp" // added because included in LRMWP via DG header 
#include "CompileTimeConfig.hpp"
// #ifdef ENABLE_DG
// 	#include "model/RSMagneticCaptureModelDG.hpp"
// #endif
#include "LoggingUtils.hpp"
#include "Logging.hpp"


namespace cadet
{
namespace model
{

	IUnitOperation* selectAxialFlowDiscretizationRSMCM(UnitOpIdx uoId, IParameterProvider& paramProvider)
	{
		IUnitOperation* model = nullptr;

		paramProvider.pushScope("discretization");

		if (paramProvider.exists("SPATIAL_METHOD")) {

			const std::string discName = paramProvider.getString("SPATIAL_METHOD");

// #ifdef ENABLE_DG
// 			if(discName == "DG")
// 				model = new RSMagneticCaptureModelDG(uoId);
// 			else if (discName == "FV")
// #else
			if (discName == "FV")
// #endif
				model = createAxialFVRSMCM(uoId);
			else
			{
				LOG(Error) << "Unknown discretization type " << discName << " for unit " << uoId;
			}
		}
		else {
			model = createAxialFVRSMCM(uoId);
		}

		paramProvider.popScope();

		return model;
	}

	IUnitOperation* selectRadialFlowDiscretizationRSMCM(UnitOpIdx uoId, IParameterProvider& paramProvider)
	{
		IUnitOperation* model = nullptr;

		paramProvider.pushScope("discretization");

		if (paramProvider.exists("SPATIAL_METHOD")) {

			const std::string discName = paramProvider.getString("SPATIAL_METHOD");

			if (discName == "DG")
			{
				LOG(Error) << "Radial flow not implemented for DG discretization yet, was called for unit " << uoId;
			}
			else if (discName == "FV")
				model = createRadialFVRSMCM(uoId);
			else
			{
				LOG(Error) << "Unknown discretization type " << discName << " for unit " << uoId;
			}
		}
		else {
			model = createRadialFVRSMCM(uoId);
		}

		paramProvider.popScope();

		return model;
	}

void registerRSMagneticCaptureModel(std::unordered_map<std::string, std::function<IUnitOperation* (UnitOpIdx, IParameterProvider&)>>& models)
{
	typedef RSMagneticCaptureModel<parts::AxialConvectionDispersionOperatorBase> AxialRSMCM;
	typedef RSMagneticCaptureModel<parts::RadialConvectionDispersionOperatorBase> RadialRSMCM;

// #ifdef ENABLE_DG
// 	models[RSMagneticCaptureModelDG::identifier()] = selectAxialFlowDiscretizationRSMCM;
// 	models["RSMCM_DG"] = selectAxialFlowDiscretizationRSMCM;
// #endif

	models[AxialRSMCM::identifier()] = selectAxialFlowDiscretizationRSMCM;
	models["RSMCM"] = selectAxialFlowDiscretizationRSMCM;
	models["DPFR"] = selectAxialFlowDiscretizationRSMCM;

	models[RadialRSMCM::identifier()] = selectRadialFlowDiscretizationRSMCM;
	models["RRSMCM"] = selectRadialFlowDiscretizationRSMCM;
}

}  // namespace model

}  // namespace cadet