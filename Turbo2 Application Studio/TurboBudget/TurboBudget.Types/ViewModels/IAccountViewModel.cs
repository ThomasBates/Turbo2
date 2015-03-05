using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TurboBudget.Types
{
    public interface IAccountViewModel
    {
        decimal Planned { get; set; }
        decimal Actual { get; set; }
        decimal Pending { get; set; }
    }
}
