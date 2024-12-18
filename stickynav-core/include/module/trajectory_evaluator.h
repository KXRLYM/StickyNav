#ifndef STICKYNAV_MODULE_TRAJECTORY_EVALUATOR_H_
#define STICKYNAV_MODULE_TRAJECTORY_EVALUATOR_H_

#include <memory>
#include <string>

#include <Eigen/Core>
#include <data/bounding_volume.h>
#include <data/trajectory_segment.h>
#include <data/visualization_markers.h>

namespace stickynav_planning
{

  // Forward declaration
  class CostComputer;
  class ValueComputer;
  class NextSelector;
  class EvaluatorUpdater;

  // Base class for trajectory evaluators to provide uniform interface with other
  // classes
  class TrajectoryEvaluator : public Module
  {
  public:
    explicit TrajectoryEvaluator(BasePlanner &planner); // NOLINT

    virtual ~TrajectoryEvaluator() = default;

    // compute the gain of a TrajectorySegment
    virtual bool computeGain(TrajectorySegment *traj_in) = 0;

    // compute the cost of a TrajectorySegment
    virtual bool computeCost(TrajectorySegment *traj_in);

    // compute the Value of a segment with known cost and gain
    virtual bool computeValue(TrajectorySegment *traj_in);

    // return the index of the most promising child segment
    virtual int selectNextBest(TrajectorySegment *traj_in);

    // Update an existing segment when a new trajectory is executed, return true
    // to keep segment alive, false to kill it
    virtual bool updateSegment(TrajectorySegment *segment);

    // Implement this method to allow visualization of the information gain during
    // simulation
    virtual void visualizeTrajectoryValue(VisualizationMarkers *markers,
                                          const TrajectorySegment &trajectory) {}

    void setupFromParamMap(Module::ParamMap *param_map) override;

  protected:
    // bounding volume of interesting target
    std::unique_ptr<BoundingVolume> bounding_volume_;

    // params
    std::string p_cost_args_;
    std::string p_value_args_;
    std::string p_next_args_;
    std::string p_updater_args_;

    // default modules
    std::unique_ptr<CostComputer> cost_computer_;
    std::unique_ptr<ValueComputer> value_computer_;
    std::unique_ptr<NextSelector> next_selector_;
    std::unique_ptr<EvaluatorUpdater> evaluator_updater_;
  };

  // Abstract encapsulation for default/modular implementations of the computeCost
  // method
  class CostComputer : public Module
  {
  public:
    explicit CostComputer(BasePlanner &planner) : Module(planner) {}

    virtual bool computeCost(TrajectorySegment *traj_in) = 0;
  };

  // Abstract encapsulation for default/modular implementations of the
  // computeValue method
  class ValueComputer : public Module
  {
  public:
    explicit ValueComputer(BasePlanner &planner) : Module(planner) {}

    virtual bool computeValue(TrajectorySegment *traj_in) = 0;
  };

  // Abstract encapsulation for default/modular implementations of the
  // selectNextBest method
  class NextSelector : public Module
  {
  public:
    explicit NextSelector(BasePlanner &planner) : Module(planner) {}

    virtual int selectNextBest(TrajectorySegment *traj_in) = 0;
  };

  // Abstract encapsulation for default/modular implementations of the
  // updateSegments method
  class EvaluatorUpdater : public Module
  {
  public:
    explicit EvaluatorUpdater(BasePlanner &planner) : Module(planner) {}

    virtual bool updateSegment(TrajectorySegment *segment) = 0;
  };

} // namespace stickynav_planning
#endif // STICKYNAV_MODULE_TRAJECTORY_EVALUATOR_H_