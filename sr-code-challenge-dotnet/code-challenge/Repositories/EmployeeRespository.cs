using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using challenge.Models;
using Microsoft.Extensions.Logging;
using Microsoft.EntityFrameworkCore;
using challenge.Data;

namespace challenge.Repositories
{
    public class EmployeeRespository : IEmployeeRepository
    {
        private readonly EmployeeContext _employeeContext;
        private readonly ILogger<IEmployeeRepository> _logger;

        public EmployeeRespository(ILogger<IEmployeeRepository> logger, EmployeeContext employeeContext)
        {
            _employeeContext = employeeContext;
            _logger = logger;
        }

        public Employee Add(Employee employee)
        {
            employee.EmployeeId = Guid.NewGuid().ToString();
            _employeeContext.Employees.Add(employee);
            return employee;
        }

        public Employee GetById(string id)
        {
            return _employeeContext.Employees.SingleOrDefault(e => e.EmployeeId == id);
        }

        public Task SaveAsync()
        {
            return _employeeContext.SaveChangesAsync();
        }

        public Employee Remove(Employee employee)
        {
            return _employeeContext.Remove(employee).Entity;
        }

        public IList<string> GetDirectReportsById(string id)
        {

            return _employeeContext.Employees.Where(x => x.EmployeeId == id).Select(x => new { EmployeeId = x.EmployeeId, DirectReports = x.DirectReports }).SingleOrDefault().DirectReports.Select(x => x.EmployeeId).ToList();


        }

        public Compensation Add(Compensation compensation)
        {
            compensation.Employee.EmployeeId = Guid.NewGuid().ToString();
            _employeeContext.Compensations.Add(compensation);
            return compensation;
        }

        public Compensation GetCompensationById(string id)
        {
            return _employeeContext.Compensations.Where(e => e.Employee.EmployeeId == id).Select(c => new Compensation
            {
                CompensationId = c.CompensationId,
                Employee = c.Employee,
                Salary = c.Salary,
                EffectiveDate = c.EffectiveDate
            }).SingleOrDefault();
        }
    }
}
