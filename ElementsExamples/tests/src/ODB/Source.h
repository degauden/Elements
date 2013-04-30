/**
 * @file Source.h
 *
 * @author Pierre Dubath
 *
 * Created on: Jan 31, 2013
 */

#ifndef SOURCE_H_
#define SOURCE_H_

#include <string>

#include <odb/core.hxx>


/**
 * The Source class include all information related to a sky source
 *
 */

///#pragma db object
class Source {
public:
  /// constructor with member assignment, without maps and without catalog "back pointer"

  Source (){}
  Source(long source_id, double ra, double dec)
    : m_source_id(source_id), m_ra (ra), m_dec(dec)
  {};

  virtual ~Source() {
  }

  /**
   * Getter
   *
   * @return The source ID
   *      source ID are defined independently in each survey
   */
  //#pragma db get(getSourceId)
  long getSourceId() const {
    return m_source_id;
  }

  /**
   * Getter
   *
   * @return The right ascension in degree from 0 to 360 degree
   */
  //#pragma db get(getRa)
  double getRa() const {
    return m_ra;
  }

  /**
   * Getter
   *
   * @return The declination in degree from -90 to +90 degree
   */
  ///#pragma db get(getDec)
  double getDec() const {
    return m_dec;
  }

private:
  friend class odb::access;

  /// source identification (attributed in the survey)
  ///#pragma db id
  long m_source_id;

  // #pragma db id auto
  // unsigned long id_;

  /// source right ascension
  double m_ra;
  /// source declination
  double m_dec;

};

#pragma db object(Source)
#pragma db member(Source::m_source_id) id

#endif /* SOURCE_H_ */
