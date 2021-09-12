using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace challenge.Models
{
    public class Employee
    {
        public String EmployeeId { get; set; }
        public String FirstName { get; set; }
        public String LastName { get; set; }
        public String Position { get; set; }
        public String Department { get; set; }
        public virtual List<Employee> DirectReports { get; set; }
    }

    public class ReportingStructure
    {
        public Employee Employee { get; set; }

        public int NumberOfReports { get; set; }

    }

    public class Compensation
    {
        public virtual string CompensationId { get; set; }
        public Employee Employee { get; set; }
        public double Salary { get; set; }
        public DateTime? EffectiveDate { get; set; }
     
    }
}
